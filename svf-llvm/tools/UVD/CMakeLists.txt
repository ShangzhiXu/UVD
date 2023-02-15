if(DEFINED IN_SOURCE_BUILD)
  add_llvm_tool(uvd uvd.cpp)
else()
  add_executable(uvd uvd.cpp)

  target_link_libraries(uvd SvfLLVM ${llvm_libs})
 # message(${llvm_libs})
  set_target_properties(uvd PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                         ${CMAKE_BINARY_DIR}/bin)
endif()
