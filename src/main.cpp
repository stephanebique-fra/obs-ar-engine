#include "Application.hpp"

int main()
{
    Application app;

    if (!app.initialize())
        return -1;

    app.run();

    app.shutdown();

    return 0;
}