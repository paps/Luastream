#include <boost/cstdlib.hpp>
#include "Server.hpp"

int main(int ac, char** av)
{
    Server::Server s;
    return s.Run(ac, av);
}
