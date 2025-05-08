cmake_minimum_required(VERSION 3.10)

project(factory)

add_executable(simple_factory simple_factory.cc)
target_link_libraries(simple_factory
  glog
)

add_executable(factory_method factory_method.cc)
target_link_libraries(factory_method
  glog
)

add_executable(abstrack_factory abstact_factory.cc)
target_link_libraries(abstrack_factory
  glog
)

add_executable(template_factory template_factory.cc)
target_link_libraries(template_factory
  glog
)

add_executable(register_factory register.cc test_register.cc)
target_link_libraries(register_factory
  glog
)