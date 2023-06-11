#include <thread>
#include <chrono>

void ExecuteBypassSkript()
{

    MessageBox(NULL, "Ola", "ola", MB_OK);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return;
}