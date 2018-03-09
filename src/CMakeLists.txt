project(simple-jit)

add_executable(${PROJECT_NAME} main.cpp)

llvm_map_components_to_libnames(llvm_libs
    core
    orcjit
    native
)

target_link_libraries(${PROJECT_NAME}
    ${llvm_libs}
)
