// ###############################################################################
// 
// Visitlab Playground - Visualization & Computer Graphics Toolkit.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// 
// ################################################################################

#include "application.hpp"
#include <functional>
#include <set>

template <class T>
void hash_combine(std::size_t& seed, const T& v) {
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
	template <typename S, typename T>
	struct hash<pair<S, T>> {
		size_t operator()(const pair<S, T>& v) const {
			size_t seed = 0;
			::hash_combine(seed, v.first);
			::hash_combine(seed, v.second);
			return seed;
		}
	};
}

bool ParticleSystem::CellId::operator==(CellId const& other) const {
	return x == other.x && y == other.y && z == other.z;
}

size_t ParticleSystem::CellIdHash::operator()(CellId const& id) const {
	size_t seed = 0;
	hash_combine(seed, id.x);
	hash_combine(seed, id.y);
	hash_combine(seed, id.z);
	return seed;
}

ParticleSystem::ParticleSystem()
	: cells()
	, current_cells(&cells.at(0))
	, future_cells(&cells.at(1))
	, planes()
	, time_buffer(0.0f) {
}

float const ParticleSystem::max_time_buffer = 1.0f / 25.0f;
float const ParticleSystem::time_step_internal = 0.0025f;
float const ParticleSystem::time_step_single = time_step_internal;
float const ParticleSystem::particle_radius = 0.1f;
float const ParticleSystem::particle_mass = 0.1f;
float const ParticleSystem::particle_viscous_drag_coef = 0.001f;
float const ParticleSystem::fluid_force_reach = 3.0f * particle_radius;
float const ParticleSystem::cell_size = fluid_force_reach;
float const ParticleSystem::gravity_accel = 8.0f;
glm::vec3 ParticleSystem::gravity_accel_dir = normalize(glm::vec3(0.0f, -1.0f, 0.0f));
glm::vec2 const ParticleSystem::restitution_coef_fluid = glm::vec2(0.5f, 0.5f);
glm::vec2 const ParticleSystem::restitution_coef_boundary = glm::vec2(0.5f, 0.5f);
int const ParticleSystem::num_collision_response_iterations = 5;
float const ParticleSystem::lennard_jones_m = 4.0f;
float const ParticleSystem::lennard_jones_n = 2.0f;
float const ParticleSystem::lennard_jones_k = 10.0f;

ParticleSystem::~ParticleSystem() {
	clear_particles();
}

void ParticleSystem::clear_particles() {
	for (auto& id_and_cells : *current_cells)
		for (Particle* particle : id_and_cells.second)
			delete particle;
	current_cells->clear();
}

void ParticleSystem::insert_particle(glm::vec3 const& position, glm::vec3 const& velocity) {
	(*current_cells)[to_cell_id(position)].push_back(new Particle{ position, velocity, glm::zero<glm::vec3>() });
}

void ParticleSystem::insert_boundary(glm::vec3 const& origin, glm::vec3 const& unit_normal) {
	planes.push_back(BoundaryPlane{ origin, unit_normal, dot(unit_normal, origin) });
}

void ParticleSystem::update(float const time_step) {
	//update_internal(time_step);
	//update_internal(time_step_internal);
	for (time_buffer = std::min(max_time_buffer, time_buffer + time_step); time_buffer >= time_step_internal;
		time_buffer -= time_step_internal)
		update_internal(time_step_internal);
}

void ParticleSystem::update_internal(float const time_step) {
	static ParticleCollisions collisions_particles;
	static BoundaryCollisions collisions_boundary;

	// We apply forces to each particle and also forces between particles.
	// Since we are iterating over (pairs) of particles we also detect and
	// record collisions. We resolve collisions later.

	for (auto& id_and_cells : *current_cells)
		for (Particle* particle : id_and_cells.second) {
			particle->force = gravity_accel * gravity_accel_dir;
			particle->force -= particle_viscous_drag_coef * particle->velocity;

			for (BoundaryPlane const& plane : planes) {
				float const distance = dot(plane.unit_normal, particle->position) - plane.dot_normal_origin - particle_radius;
				if (distance < 0.0f)
					collisions_boundary.push_back({ particle, &plane, -distance });
			}
		}

	auto const process_pair_of_particles = [](Particle* const p1, Particle* const p2) -> void
	{
		glm::vec3 u = p1->position - p2->position;
		float u_len = length(u);
		if (u_len < particle_radius * 0.1f) {
			u_len = particle_radius * 0.1f;
			u = -u_len * gravity_accel_dir;
		}
		glm::vec3 const unit_u = (1.0f / u_len) * u;

		if (u_len < fluid_force_reach) {
			float x_normed = std::max(u_len / (2.0f * particle_radius), 0.25f);
			float magnitude = lennard_jones_k / std::pow(x_normed, lennard_jones_m) - lennard_jones_k / std::pow(
				x_normed, lennard_jones_n);
			p1->force += magnitude * unit_u;
			p2->force -= magnitude * unit_u;
		}

		float const distance = u_len - 2.0f * particle_radius;
		if (distance < 0.0f)
			collisions_particles.push_back({ p1, p2, unit_u, -distance });
	};

	for (auto& id_and_cells : *current_cells) {
		std::vector<std::pair<ParticlesInCells::mapped_type::iterator, ParticlesInCells::mapped_type::iterator>> neighbours;
		{
			static std::vector<std::tuple<int, int, int>> const neighbour_indices = {
				{0, 0, -1},
				{0, 1, -1},
				{0, 1, 0},
				{0, 1, 1},
				{1, -1, -1},
				{1, -1, 0},
				{1, -1, 1},
				{1, 0, -1},
				{1, 0, 0},
				{1, 0, 1},
				{1, 1, -1},
				{1, 1, 0},
				{1, 1, 1}
			};
			for (std::tuple<int, int, int> const& indices : neighbour_indices) {
				auto const it = current_cells->find({
					id_and_cells.first.x + std::get<0>(indices),
					id_and_cells.first.y + std::get<1>(indices),
					id_and_cells.first.z + std::get<2>(indices)
					});
				if (it != current_cells->end())
					neighbours.push_back({ it->second.begin(), it->second.end() });
			}
		}

		for (auto it = id_and_cells.second.begin(); it != id_and_cells.second.end(); ++it) {
			for (auto other_it = std::next(it); other_it != id_and_cells.second.end(); ++other_it)
				process_pair_of_particles(*it, *other_it);
			for (auto const& begin_and_end : neighbours)
				for (auto other_it = begin_and_end.first; other_it != begin_and_end.second; ++other_it)
					process_pair_of_particles(*it, *other_it);
		}
	}

	// Now we resolve collisions.

	for (int i = 0; i != num_collision_response_iterations; ++i) {
		for (CollisionParticles const& collision : collisions_particles) {
			glm::vec3 dv = collision.p1->velocity - collision.p2->velocity;
			float const dot_u_dv = dot(collision.unit_distance_vector, dv);
			if (dot_u_dv < 0.0f) {
				float const magnitude = -0.5f * (1.0f + restitution_coef_fluid.x) * dot_u_dv;
				collision.p1->velocity += magnitude * collision.unit_distance_vector;
				collision.p2->velocity -= magnitude * collision.unit_distance_vector;
			}

			glm::vec3 df = collision.p1->force - collision.p2->force;
			float const dot_u_df = dot(collision.unit_distance_vector, df);
			if (dot_u_df < 0.0f) {
				float const magnitude = -0.5f * (1.0f + restitution_coef_fluid.y) * dot_u_df;
				collision.p1->force += magnitude * collision.unit_distance_vector;
				collision.p2->force -= magnitude * collision.unit_distance_vector;
			}
		}

		for (CollisionBoundary const& collision : collisions_boundary) {
			Particle& particle = *collision.particle;
			BoundaryPlane const& plane = *collision.boundary;

			float const dot_normal_force = dot(plane.unit_normal, particle.force);
			if (dot_normal_force < 0.0f)
				particle.force += (-(1.0f + restitution_coef_boundary.y) * dot_normal_force) * plane.unit_normal;

			float const dot_normal_velocity = dot(plane.unit_normal, particle.velocity);
			if (dot_normal_velocity < 0.0f)
				particle.velocity += (-(1.0f + restitution_coef_boundary.x) * dot_normal_velocity) * plane.unit_normal;
		}
	}

	collisions_particles.clear();
	collisions_boundary.clear();

	// Next we integrate states of particles. We use Euler's numerical integration.

	for (auto& id_and_cells : *current_cells)
		for (Particle* particle : id_and_cells.second) {
			particle->position += time_step * particle->velocity;
			particle->velocity += (time_step / particle_mass) * particle->force;
		}

	// And we hash moved particles to grid cell; preparation for the next update.

	swap_cells();
}

void ParticleSystem::get_triangles_of_particles(
	glm::vec3 const& camera_I,
	glm::vec3 const& camera_J,
	float const particle_radius_scale,
	std::vector<float>& coords,
	std::vector<uint32_t>& indices,
	std::vector<float>& tex_coords
) const {
	static const std::size_t num_coords_per_vertex = 3U;
	static const std::size_t num_vertices_per_particle = 4U;
	static const std::size_t num_indices_per_particle = 6U;
	static const std::size_t estimated_average_particles_in_cell = 10U;

	const float particle_radius_render = particle_radius_scale * particle_radius;
	const std::vector<std::tuple<float, float, float, float>> shifts = {
		{-particle_radius_render, -particle_radius_render, 0.0f, 0.0f},
		{particle_radius_render, -particle_radius_render, 1.0f, 0.0f},
		{particle_radius_render, particle_radius_render, 1.0f, 1.0f},
		{-particle_radius_render, particle_radius_render, 0.0f, 1.0f}
	};

	coords.clear();
	coords.reserve(num_coords_per_vertex * num_vertices_per_particle * (current_cells->size() * estimated_average_particles_in_cell));
	indices.clear();
	indices.reserve(num_indices_per_particle * (current_cells->size() * estimated_average_particles_in_cell));
	for (auto& id_and_cells : *current_cells)
		for (Particle* particle : id_and_cells.second) {
			uint32_t const start_index = static_cast<uint32_t>(coords.size()) / num_coords_per_vertex;
			for (std::tuple<float, float, float, float> const& shift : shifts) {
				glm::vec3 const vertex = particle->position + get < 0 >(shift) * camera_I + get < 1 >(shift) * camera_J;
				coords.push_back(vertex.x);
				coords.push_back(vertex.y);
				coords.push_back(vertex.z);

				tex_coords.push_back(get < 2 >(shift));
				tex_coords.push_back(get < 3 >(shift));
			}
			indices.push_back(start_index + 0U);
			indices.push_back(start_index + 1U);
			indices.push_back(start_index + 2U);
			indices.push_back(start_index + 0U);
			indices.push_back(start_index + 2U);
			indices.push_back(start_index + 3U);
		}
}

void ParticleSystem::swap_cells() {
	future_cells->clear();
	for (auto& id_and_cells : *current_cells)
		for (Particle* particle : id_and_cells.second)
			(*future_cells)[to_cell_id(particle->position)].push_back(particle);
	std::swap(current_cells, future_cells);
}

ParticleSystem::CellId ParticleSystem::to_cell_id(glm::vec3 const& position) {
	return CellId(static_cast<int>(position.x / cell_size), static_cast<int>(position.y / cell_size),
		static_cast<int>(position.z / cell_size));
}

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
	: PA213Application(initial_width, initial_height, arguments)
	, particles()
	, gpu_program_billboards(lecture_shaders_path / "billboard.vert", lecture_shaders_path / "billboard.frag")
	, gpu_program_depth(lecture_shaders_path / "depth.vert", lecture_shaders_path / "depth.frag")
	, gpu_program_smooth(framework_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "smooth.frag")
	, gpu_program_normals(framework_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "normals.frag")
	, gpu_program_lighting(framework_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "lighting.frag")
	, gpu_program_ground(lecture_shaders_path / "ground.vert", lecture_shaders_path / "ground.frag")
	, texture_fluid_depth(nullptr)
	, texture_fluid_colour_aux(0)
	, texture_fluid_depth_smooth(nullptr)
	, texture_fluid_depth_smooth2(nullptr)
	, texture_fluid_colour_smooth_aux(0)
	, texture_fluid_depth_normals_aux(nullptr)
	, texture_fluid_colour_normals(0)
	, fbo_fluid_depth(0)
	, fbo_fluid_smooth(0)
	, fbo_fluid_smooth2(0)
	, fbo_fluid_normals(0)
	, geometry_ground(
		GL_TRIANGLES,
		[]() -> std::vector<float>
		{
			glm::vec2 const coords = ParticleSystem::particle_radius * glm::vec2(ground_size);
			static std::vector<float> vertices = {
				// Front face
				-coords.x, 2.0f, coords.y,
				-coords.x, 0.0f, coords.y,
				coords.x, 0.0f, coords.y,
				coords.x, 2.0f, coords.y,
				// Right face
				coords.x, 2.0f, coords.y,
				coords.x, 0.0f, coords.y,
				coords.x, 0.0f, -coords.y,
				coords.x, 2.0f, -coords.y,
				// Back face
				coords.x, 2.0f, -coords.y,
				coords.x, 0.0f, -coords.y,
				-coords.x, 0.0f, -coords.y,
				-coords.x, 2.0f, -coords.y,
				// Left face
				-coords.x, 2.0f, -coords.y,
				-coords.x, 0.0f, -coords.y,
				-coords.x, 0.0f, coords.y,
				-coords.x, 2.0f, coords.y,
				// Bottom face
				-coords.x, 0.0f, coords.y,
				-coords.x, 0.0f, -coords.y,
				coords.x, 0.0f, -coords.y,
				coords.x, 0.0f, coords.y,
			};
			return vertices;
		}(),
			[]() -> std::vector<uint32_t>
		{
			static std::vector<uint32_t> indices = {
				0, 2, 1, 0, 3, 2,       // Front face
				4, 6, 5, 4, 7, 6,       // Right face
				8, 10, 9, 8, 11, 10,    // Back face
				12, 14, 13, 12, 15, 14, // Left face
				16, 18, 17, 16, 19, 18, // Bottom face
			};
			return indices;
		}(),
			[]() -> std::vector<float>
		{
			static std::vector<float> normals = {
				// Front face
				0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,
				// Right face
				-1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f,
				// Back face
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				// Left face
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				// Bottom face
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
			};
			return normals;
		}()
			)
	, paused(false)
			, do_single_step(false)
			, show_ground(true)
			, show_billboards(false)
			, show_depth_texture(false)
			, show_depth_smooth_texture(false)
			, show_normals_texture(false)
			, smoothing_radius(5)
			, num_smooting_passes(2)
			, smooting_gauss_sigma(0.9f)
			, smooting_depth_falloff(200.0f)
			, diffuse_coef(0.6f)
			, specular_coef(0.6f)
			, specular_exponent(60.0f)
			, particle_radius_scale(1.0f)
			, fluid_gen_hight(30)
			, use_fixed_fluid_render_target_size(true) {
			glm::vec2 const coords = ParticleSystem::particle_radius * glm::vec2(ground_size);
			particles.insert_boundary(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			particles.insert_boundary(glm::vec3(-coords.x, 0, 0), glm::vec3(1, 0, 0));
			particles.insert_boundary(glm::vec3(coords.x, 0, 0), glm::vec3(-1, 0, 0));
			particles.insert_boundary(glm::vec3(0, 0, -coords.y), glm::vec3(0, 0, 1));
			particles.insert_boundary(glm::vec3(0, 0, coords.y), glm::vec3(0, 0, -1));
			generate_particles();

			create_fluid_render_targets();

			camera.set_eye_position(-3.14f / 4.0f, 3.14f / 4.0f, 15);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		Application::~Application() {
			if (texture_fluid_depth != nullptr)
				destroy_fluid_render_targets();
		}

		void Application::compile_shaders() {

			gpu_program_billboards = ShaderProgram(lecture_shaders_path / "billboard.vert", lecture_shaders_path / "billboard.frag");
			gpu_program_depth = ShaderProgram(lecture_shaders_path / "depth.vert", lecture_shaders_path / "depth.frag");
			gpu_program_smooth = ShaderProgram(framework_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "smooth.frag");
			gpu_program_normals = ShaderProgram(framework_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "normals.frag");
			gpu_program_lighting = ShaderProgram(framework_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "lighting.frag");
			gpu_program_ground = ShaderProgram(lecture_shaders_path / "ground.vert", lecture_shaders_path / "ground.frag");

			std::cout << "Shaders are reloaded." << std::endl;
		}

		glm::i32vec2 const Application::ground_size(50, 8);
		glm::i32vec2 const Application::desired_fluid_size(4, 4);
		glm::i32vec3 const Application::desired_fluid_origin_shift(-ground_size.x / 2 + desired_fluid_size.x, 1,
			-ground_size.y / 2 + desired_fluid_size.y);
		glm::vec3 const Application::light_direction = normalize(glm::vec3(1, 1, 1));
		glm::vec4 const Application::light_color(0.9f, 0.9f, 0.9f, 1);
		glm::vec4 const Application::ambient_color(0.1f, 0.1f, 0.1f, 1);
		glm::i32vec2 const Application::fixed_fluid_render_target_size(512, 512);

		void Application::generate_particles() {
			static glm::i32 const num_x = std::max(1, std::min(desired_fluid_size.x, ground_size.x - 1));
			static glm::i32 const num_z = std::max(1, std::min(desired_fluid_size.y, ground_size.y - 1));
			static glm::vec3 const origin = (2.0f * ParticleSystem::particle_radius) * glm::vec3(
				desired_fluid_origin_shift.x,
				desired_fluid_origin_shift.y,
				desired_fluid_origin_shift.z
			);
			for (std::size_t i = 0U; i < num_x; ++i)
				for (std::size_t j = 0U; j < fluid_gen_hight; ++j)
					for (std::size_t k = 0U; k < num_z; ++k) {
						float const ext = j % 2 == 0 ? -0.25f : 0.25f;
						glm::vec3 shift = (2.0f * ParticleSystem::particle_radius) * glm::vec3(
							i - num_x / 2.0f + ext, j * 0.75f, k - num_z / 2.0f + ext);
						particles.insert_particle(origin + shift, glm::vec3(0, 0, 0));
					}
		}

		void Application::create_fluid_render_targets() {
			GLsizei const tex_width = use_fixed_fluid_render_target_size ? fixed_fluid_render_target_size.x : width;
			GLsizei const tex_height = use_fixed_fluid_render_target_size ? fixed_fluid_render_target_size.y : height;

			texture_fluid_depth = std::make_unique<Texture>(tex_width, tex_height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT);
			texture_fluid_depth->set_texture_2d_parameters(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
			glCreateTextures(GL_TEXTURE_2D, 1, &texture_fluid_colour_aux);
			glTextureStorage2D(texture_fluid_colour_aux, 1, GL_RGB32F, tex_width, tex_height);

			texture_fluid_depth_smooth = std::make_unique<Texture>(tex_width, tex_height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT);
			texture_fluid_depth_smooth->set_texture_2d_parameters(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
			glCreateTextures(GL_TEXTURE_2D, 1, &texture_fluid_colour_smooth_aux);
			glTextureStorage2D(texture_fluid_colour_smooth_aux, 1, GL_RGB32F, tex_width, tex_height);

			texture_fluid_depth_smooth2 = std::make_unique<Texture>(tex_width, tex_height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT);
			texture_fluid_depth_smooth2->set_texture_2d_parameters(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
			glCreateTextures(GL_TEXTURE_2D, 1, &texture_fluid_colour_smooth_aux);
			glTextureStorage2D(texture_fluid_colour_smooth_aux, 1, GL_RGB32F, tex_width, tex_height);

			texture_fluid_depth_normals_aux = std::make_unique<Texture>(tex_width, tex_height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT);
			glCreateTextures(GL_TEXTURE_2D, 1, &texture_fluid_colour_normals);
			glTextureStorage2D(texture_fluid_colour_normals, 1, GL_RGB32F, tex_width, tex_height);

			glGenFramebuffers(1, &fbo_fluid_depth);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluid_depth);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_fluid_depth->get_opengl_object(), 0);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_fluid_colour_aux, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glGenFramebuffers(1, &fbo_fluid_smooth);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluid_smooth);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_fluid_depth_smooth->get_opengl_object(), 0);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_fluid_colour_smooth_aux, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glGenFramebuffers(1, &fbo_fluid_smooth2);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluid_smooth2);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_fluid_depth_smooth2->get_opengl_object(), 0);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_fluid_colour_smooth_aux, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glGenFramebuffers(1, &fbo_fluid_normals);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluid_normals);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_fluid_depth_normals_aux->get_opengl_object(), 0);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_fluid_colour_normals, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void Application::destroy_fluid_render_targets() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDeleteFramebuffers(1, &fbo_fluid_depth);
			glDeleteFramebuffers(1, &fbo_fluid_smooth);
			glDeleteFramebuffers(1, &fbo_fluid_smooth2);
			glDeleteFramebuffers(1, &fbo_fluid_normals);

			texture_fluid_depth.release();
			glDeleteTextures(1, &texture_fluid_colour_aux);
			texture_fluid_depth_smooth.release();
			texture_fluid_depth_smooth2.release();
			glDeleteTextures(1, &texture_fluid_colour_smooth_aux);
			texture_fluid_depth_normals_aux.release();
			glDeleteTextures(1, &texture_fluid_colour_normals);
		}

		void Application::rebuild_fluid_render_targets_if_needed() {
			if (texture_fluid_depth == nullptr)
				create_fluid_render_targets();
			else {
				GLsizei const tex_width = use_fixed_fluid_render_target_size ? fixed_fluid_render_target_size.x : width;
				GLsizei const tex_height = use_fixed_fluid_render_target_size ? fixed_fluid_render_target_size.y : height;
				if (texture_fluid_depth->get_width() != tex_width || texture_fluid_depth->get_height() != tex_height) {
					destroy_fluid_render_targets();
					create_fluid_render_targets();
				}
			}
		}

		void Application::update(float time_step_milliseconds) {
			if (paused && !do_single_step)
				return;

			float const time_step = do_single_step ? ParticleSystem::time_step_single : time_step_milliseconds / 1000.0f;
			particles.update(time_step);

			do_single_step = false;
		}

		void Application::render() {
			if (!gpu_program_ground.is_valid())
				return;

			rebuild_fluid_render_targets_if_needed();

			float const near_plane = 0.1f;

			glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), near_plane,
				100.0f);
			glm::mat4 view_matrix = camera.get_view_matrix();
			glm::mat4 model_matrix = glm::identity<glm::mat4>();

			glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
			activate_render_buffer();

			if (show_ground) {
				activate_viewport();
				gpu_program_ground.use();
				glUniformMatrix4fv(gpu_program_ground.get_uniform_location("projection"), 1, GL_FALSE, value_ptr(projection_matrix));
				glUniformMatrix4fv(gpu_program_ground.get_uniform_location("view"), 1, GL_FALSE, value_ptr(view_matrix));
				glUniformMatrix4fv(gpu_program_ground.get_uniform_location("model"), 1, GL_FALSE, value_ptr(model_matrix));
				glUniform3fv(gpu_program_ground.get_uniform_location("light_direction"), 1, value_ptr(light_direction));
				geometry_ground.draw();
			}

			std::vector<float> coords;
			std::vector<uint32_t> indices;
			std::vector<float> tex_coords;
			glm::mat3 rotation(transpose(view_matrix));
			particles.get_triangles_of_particles(rotation * glm::vec3(1, 0, 0), rotation * glm::vec3(0, 1, 0), particle_radius_scale, coords,
				indices, tex_coords);
			if (!coords.empty()) {
				Geometry points(GL_TRIANGLES, coords, indices, {}, {}, tex_coords);

				if (show_billboards) {
					activate_viewport();
					gpu_program_billboards.use();
					glUniformMatrix4fv(gpu_program_billboards.get_uniform_location("projection"), 1, GL_FALSE, value_ptr(projection_matrix));
					glUniformMatrix4fv(gpu_program_billboards.get_uniform_location("view"), 1, GL_FALSE, value_ptr(view_matrix));
					glUniformMatrix4fv(gpu_program_billboards.get_uniform_location("model"), 1, GL_FALSE, value_ptr(model_matrix));
					glUniform3fv(gpu_program_billboards.get_uniform_location("light_direction"), 1, value_ptr(light_direction));
					points.draw();
				}
				else {
					glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
					prepare_rendering(0, 0, texture_fluid_depth->get_width(), texture_fluid_depth->get_height(), fbo_fluid_depth, true, true);
					gpu_program_depth.use();
					glUniformMatrix4fv(gpu_program_depth.get_uniform_location("projection"), 1, GL_FALSE, value_ptr(projection_matrix));
					glUniformMatrix4fv(gpu_program_depth.get_uniform_location("view"), 1, GL_FALSE, value_ptr(view_matrix));
					glUniformMatrix4fv(gpu_program_depth.get_uniform_location("model"), 1, GL_FALSE, value_ptr(model_matrix));
					glUniformMatrix4fv(gpu_program_depth.get_uniform_location("projection_matrix"), 1, GL_FALSE, value_ptr(projection_matrix));
					glUniform1f(gpu_program_depth.get_uniform_location("particle_radius"), ParticleSystem::particle_radius * particle_radius_scale);
					glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluid_depth);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_fluid_depth->get_opengl_object(), 0);
					glBindVertexArray(base_vao);
					points.draw();

					if (show_depth_texture) {
						prepare_rendering();
						/*
						display_texture(*texture_fluid_depth, true);
						/*/
						if (display_texture_program.is_valid()) {
							display_texture_program.use();
							display_texture_program.uniform("depth_texture", 0);
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, texture_fluid_colour_aux);
							glBindVertexArray(base_vao);
							glDrawArrays(GL_TRIANGLES, 0, 3);
						}
						//*/
						activate_viewport();
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
						return;
					}

					glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
					prepare_rendering(0, 0, texture_fluid_depth_smooth->get_width(), texture_fluid_depth_smooth->get_height(), fbo_fluid_smooth,
						true, true);
					gpu_program_smooth.use();
					glUniform1i(gpu_program_smooth.get_uniform_location("depth_texture"), 0);
					glUniformMatrix4fv(gpu_program_smooth.get_uniform_location("projection_matrix"), 1, GL_FALSE, value_ptr(projection_matrix));
					glUniformMatrix4fv(gpu_program_smooth.get_uniform_location("projection_matrix_inverted"), 1, GL_FALSE,
						value_ptr(inverse(projection_matrix)));
					glUniform1f(gpu_program_smooth.get_uniform_location("particle_radius"),
						ParticleSystem::particle_radius * particle_radius_scale);
					glUniform1i(gpu_program_smooth.get_uniform_location("smoothing_radius"), smoothing_radius);
					glUniform1f(gpu_program_smooth.get_uniform_location("smooting_gauss_sigma"), smooting_gauss_sigma);
					glUniform1f(gpu_program_smooth.get_uniform_location("smooting_depth_falloff"), smooting_depth_falloff);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texture_fluid_depth->get_opengl_object());
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, texture_fluid_colour_aux);
					glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluid_smooth);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_fluid_depth_smooth->get_opengl_object(), 0);
					glDrawArrays(GL_TRIANGLES, 0, 3);

					for (int i = 0; i != num_smooting_passes; ++i) {
						prepare_rendering(0, 0, texture_fluid_depth_smooth2->get_width(), texture_fluid_depth_smooth2->get_height(),
							fbo_fluid_smooth2, true, true);
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, texture_fluid_depth_smooth->get_opengl_object());
						glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluid_smooth2);
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_fluid_depth_smooth2->get_opengl_object(),
							0);
						glDrawArrays(GL_TRIANGLES, 0, 3);

						prepare_rendering(0, 0, texture_fluid_depth_smooth->get_width(), texture_fluid_depth_smooth->get_height(), fbo_fluid_smooth,
							true, true);
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, texture_fluid_depth_smooth2->get_opengl_object());
						glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluid_smooth);
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_fluid_depth_smooth->get_opengl_object(),
							0);
						glDrawArrays(GL_TRIANGLES, 0, 3);
					}

					if (show_depth_smooth_texture) {
						prepare_rendering();
						/*
						display_texture(*texture_fluid_depth_smooth, true);
						/*/
						if (display_texture_program.is_valid()) {
							display_texture_program.use();
							display_texture_program.uniform("depth_texture", 0);
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, texture_fluid_colour_smooth_aux);
							glBindVertexArray(base_vao);
							glDrawArrays(GL_TRIANGLES, 0, 3);
						}
						//*/
						activate_viewport();
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
						return;
					}

					glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
					prepare_rendering(0, 0, texture_fluid_depth_normals_aux->get_width(), texture_fluid_depth_normals_aux->get_height(),
						fbo_fluid_normals, true, true);
					gpu_program_normals.use();
					glUniform1i(gpu_program_normals.get_uniform_location("depth_texture"), 0);
					glUniformMatrix4fv(gpu_program_normals.get_uniform_location("projection_matrix_inverted"), 1, GL_FALSE,
						value_ptr(inverse(projection_matrix)));
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texture_fluid_depth_smooth->get_opengl_object());
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, texture_fluid_colour_smooth_aux);
					glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluid_normals);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_fluid_depth_normals_aux->get_opengl_object(),
						0);
					glDrawArrays(GL_TRIANGLES, 0, 3);

					if (show_normals_texture) {
						prepare_rendering();
						if (display_texture_program.is_valid()) {
							display_texture_program.use();
							display_texture_program.uniform("depth_texture", 0);
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, texture_fluid_colour_normals);
							glBindVertexArray(base_vao);
							glDrawArrays(GL_TRIANGLES, 0, 3);
						}
						activate_viewport();
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
						return;
					}

					activate_viewport();
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					gpu_program_lighting.use();
					glUniform4fv(gpu_program_lighting.get_uniform_location("ambient_color"), 1, value_ptr(ambient_color));
					{
						glm::vec3 const light_dir = view_matrix * glm::vec4(light_direction, 0);
						glUniform3fv(gpu_program_lighting.get_uniform_location("light_direction"), 1, value_ptr(light_dir));
					}
					glUniform4fv(gpu_program_lighting.get_uniform_location("light_color"), 1, value_ptr(light_color));
					{
						glm::vec4 const coef(diffuse_coef, 0.05f * diffuse_coef, 0.1f * diffuse_coef, 1.0f);
						glUniform4fv(gpu_program_lighting.get_uniform_location("diffuse_coef"), 1, value_ptr(coef));
					}
					{
						glm::vec4 const coef(specular_coef, specular_coef, specular_coef, 1.0f);
						glUniform4fv(gpu_program_lighting.get_uniform_location("specular_coef"), 1, value_ptr(coef));
					}
					glUniform1f(gpu_program_lighting.get_uniform_location("specular_exponent"), specular_exponent);
					glUniformMatrix4fv(gpu_program_lighting.get_uniform_location("projection_matrix_inverted"), 1, GL_FALSE,
						value_ptr(inverse(projection_matrix)));
					glUniform1i(gpu_program_lighting.get_uniform_location("depth_texture"), 0);
					glUniform1i(gpu_program_lighting.get_uniform_location("normals_texture"), 1);
					glActiveTexture(GL_TEXTURE0);
					//glBindTexture(GL_TEXTURE_2D, texture_fluid_depth_smooth->get_opengl_object());
					glBindTexture(GL_TEXTURE_2D, texture_fluid_depth->get_opengl_object());
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, texture_fluid_colour_normals);
					glDrawArrays(GL_TRIANGLES, 0, 3);
				}
			}
		}

		void Application::render_ui() {
			ImGui::Begin("Fluid", nullptr);

			static bool size_set = false;
			if (!size_set) {
				const float unit = ImGui::GetFontSize();
				ImGui::SetWindowPos(ImVec2(0.0f * unit, 0.0f * unit));
				ImGui::SetWindowSize(ImVec2(16 * unit, 28.5f * unit));
				size_set = true;
			}

			ImGui::Checkbox("Pause", &paused);
			ImGui::SameLine();
			if (ImGui::Button("Restart")) {
				particles.clear_particles();
				generate_particles();
			}
			ImGui::SameLine();
			if (ImGui::Button("Single step")) {
				do_single_step = true;
				paused = true;
			}
			ImGui::PushItemWidth(7 * ImGui::GetFontSize());
			ImGui::SliderInt("Fluid hight", &fluid_gen_hight, 1, 100);
			ImGui::Checkbox("Render targets 512x512", &use_fixed_fluid_render_target_size);
			ImGui::Checkbox("Show ground", &show_ground);
			if (ImGui::Checkbox("Show billboards", &show_billboards)) {
				show_depth_texture = false;
				show_depth_smooth_texture = false;
				show_normals_texture = false;
			}
			if (ImGui::Checkbox("Show depth texture", &show_depth_texture)) {
				show_billboards = false;
				show_depth_smooth_texture = false;
				show_normals_texture = false;
			}
			if (ImGui::Checkbox("Show depth smooth texture", &show_depth_smooth_texture)) {
				show_billboards = false;
				show_depth_texture = false;
				show_normals_texture = false;
			}
			if (ImGui::Checkbox("Show normals texture", &show_normals_texture)) {
				show_billboards = false;
				show_depth_texture = false;
				show_depth_smooth_texture = false;
			}
			ImGui::PushItemWidth(7 * ImGui::GetFontSize());
			ImGui::SliderInt("Smooting passes", &num_smooting_passes, 0, 10);
			ImGui::PushItemWidth(7 * ImGui::GetFontSize());
			ImGui::SliderInt("Smooting radius", &smoothing_radius, 0, 20);
			ImGui::PushItemWidth(7 * ImGui::GetFontSize());
			ImGui::SliderFloat("Gauss sigma", &smooting_gauss_sigma, 0.1f, 10.0f);
			ImGui::PushItemWidth(7 * ImGui::GetFontSize());
			ImGui::SliderFloat("Depth falloff", &smooting_depth_falloff, 0.0f, 1000.0f);
			ImGui::PushItemWidth(7 * ImGui::GetFontSize());
			ImGui::SliderFloat("Diffuse coef", &diffuse_coef, 0.01f, 1.0f);
			ImGui::PushItemWidth(7 * ImGui::GetFontSize());
			ImGui::SliderFloat("Specular coef", &specular_coef, 0.01f, 1.0f);
			ImGui::PushItemWidth(7 * ImGui::GetFontSize());
			ImGui::SliderFloat("Specular exponent", &specular_exponent, 1.0f, 100.0f);
			ImGui::PushItemWidth(7 * ImGui::GetFontSize());
			ImGui::SliderFloat("Particle scale", &particle_radius_scale, 1.0f, 2.0f);

			ImGui::End();
		}
