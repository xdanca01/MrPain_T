/* TBD. Create overlords which create a resource and non-overlords
 * which consume it. Enforce the balance by throwing an exception on
 * exhaustion. */

class swarm;

class unit
{
    swarm &owner;
};

class overlord : unit
{
};

class zergling : unit
{
};

class swarm
{
    int _control  = 3;
    int _resource = 200;
public:
    overlord spawn_overlord();
    zergling spawn_zergling();
};

int main() /* demo */
{
    swarm s;

    std::vector< zergling > zerglings;
    std::vector< overlord > overlords;

    zerglings.emplace_back( s );
}
