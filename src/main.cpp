#include <iostream>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>

int main(int argc, char *argv[])
{

    llvm::EngineBuilder().selectTarget();
    return 0;
}
