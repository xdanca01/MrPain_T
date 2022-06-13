################################################################################
# Common Framework for Computer Graphics Courses at FI MUNI.
#
# Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
# All rights reserved.
#
# Course: PV112 (Lecture #4)
################################################################################

# Generates the lecture.
visitlab_generate_lecture(PV112 lecture_04)

# Adds additional source files.
target_sources(PV112_lecture_04 PRIVATE data.hpp)