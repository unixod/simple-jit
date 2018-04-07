#include <iostream>

// General LLVM
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/ManagedStatic.h>

// IR generation
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/BasicBlock.h>


// After the creation of any llvm::Function, it is recomended
// to verify it by calling llvm::verifyFunction.
inline void assertFunctionCorrect(const llvm::Function& f)
{
    std::string errMsg;
    llvm::raw_string_ostream errs{errMsg};
    if (llvm::verifyFunction(f, &errs) == true) {
        throw std::runtime_error{errs.str()};
    }
}

// After the creation of any llvm::Module, it is recomended
// to verify it by calling llvm::verifyModule.
inline void assertModuleCorrect(const llvm::Module& m)
{
    std::string errMsg;
    llvm::raw_string_ostream errs{errMsg};
    if (llvm::verifyModule(m, &errs) == true) {
        throw std::runtime_error{errs.str()};
    }
}

inline std::unique_ptr<llvm::Module> codegen(const std::string& moduleName, llvm::LLVMContext& llvmContext)
{
    // llvm::Module owns the memory for all of the IR that we generate, therefore LLVM APIs
    // return raw pointers (they don't own any memory).
    auto module = std::make_unique<llvm::Module>(moduleName, llvmContext);
    module->setSourceFileName("file.name");     // For IR readability.

    /* Create a function:
     *   foo(double arg) {
     *       return arg + 9;
     *   }
     */

    // First create the function's type: double(double)
    auto fooType =
            llvm::FunctionType::get(llvm::Type::getDoubleTy(llvmContext), {llvm::Type::getDoubleTy(llvmContext)}, /*not vararg*/false);

    // If we don't need the full control over the signature of our function,
    // then we may use simplier method of creation:
    //     llvm::cast<llvm::Function>(module->getOrInsertFunction("foo", fooType));
    auto foo =
            llvm::Function::Create(fooType, llvm::Function::ExternalLinkage, "foo", module.get());

    // Add a basic block to the function.
    auto bb =
            llvm::BasicBlock::Create(llvmContext, "entry", foo);

    // Get a constant.
    auto nine =
            llvm::ConstantFP::get(llvm::Type::getDoubleTy(llvmContext), 9);

    // Create an add instruction: arg + 9.
    llvm::IRBuilder<> irBuilder(bb);

    assert((foo->arg_size() == 1) && "Our function takes only one argument");
    auto arg = &*foo->arg_begin();
    arg->setName("arg");            // For IR readability.

    auto result =
            irBuilder.CreateFAdd(arg, nine, "sum");

    // Return statement.
    irBuilder.CreateRet(result);

    // Verify the correctness of created function and module.
    assertFunctionCorrect(*foo);
    assertModuleCorrect(*module);

    return module;
}

int main(int /*argc*/, char */*argv*/[])
{
    try {
        /**** These are four preliminary steps required before working with LLVM APIs. ******/
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmParser();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::llvm_shutdown_obj x;                  // RAII for llvm::llvm_shutdown.


        /**** Now we are ready to build IR... ****/

        // Every LLVM entity (Modules, Values, Types, Constants, etc.) in LLVM’s in-memory IR belongs to
        // an LLVMContext. Entities in different contexts cannot interact with each other: Modules in
        // different contexts cannot be linked together, Functions cannot be added to Modules in different
        // contexts, etc.
        llvm::LLVMContext llvmContext;

        auto module = codegen("my first module", llvmContext);


        /**** ... and use it for example for JITing ****/
        module->dump();
    }
    catch(const std::exception& ex) {
        std::cerr << "Exception has thrown, what(): " << ex.what() << '\n';
        return EXIT_FAILURE;
    }
    catch(...) {
        std::cerr << "Unknown exception has thrown\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
