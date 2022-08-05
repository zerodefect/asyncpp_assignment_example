
#include <async++.h>

enum class result
{
    OK = 0,
    FAIL = 1,
};

//////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    async::shared_task<result> task{async::make_task(result::OK)};
    return task.get()==result::OK ? 0 : 1;
}
