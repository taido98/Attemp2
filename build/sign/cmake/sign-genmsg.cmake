# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "sign: 1 messages, 0 services")

set(MSG_I_FLAGS "-Isign:/home/thanh/catkin_cpp/src/sign/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg;-Isign:/home/thanh/catkin_cpp/src/sign/msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(sign_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg" NAME_WE)
add_custom_target(_sign_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "sign" "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(sign
  "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/sign
)

### Generating Services

### Generating Module File
_generate_module_cpp(sign
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/sign
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(sign_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(sign_generate_messages sign_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg" NAME_WE)
add_dependencies(sign_generate_messages_cpp _sign_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sign_gencpp)
add_dependencies(sign_gencpp sign_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sign_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(sign
  "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/sign
)

### Generating Services

### Generating Module File
_generate_module_eus(sign
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/sign
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(sign_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(sign_generate_messages sign_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg" NAME_WE)
add_dependencies(sign_generate_messages_eus _sign_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sign_geneus)
add_dependencies(sign_geneus sign_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sign_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(sign
  "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/sign
)

### Generating Services

### Generating Module File
_generate_module_lisp(sign
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/sign
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(sign_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(sign_generate_messages sign_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg" NAME_WE)
add_dependencies(sign_generate_messages_lisp _sign_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sign_genlisp)
add_dependencies(sign_genlisp sign_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sign_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(sign
  "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/sign
)

### Generating Services

### Generating Module File
_generate_module_nodejs(sign
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/sign
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(sign_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(sign_generate_messages sign_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg" NAME_WE)
add_dependencies(sign_generate_messages_nodejs _sign_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sign_gennodejs)
add_dependencies(sign_gennodejs sign_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sign_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(sign
  "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sign
)

### Generating Services

### Generating Module File
_generate_module_py(sign
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sign
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(sign_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(sign_generate_messages sign_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/thanh/catkin_cpp/src/sign/msg/SignMsg.msg" NAME_WE)
add_dependencies(sign_generate_messages_py _sign_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sign_genpy)
add_dependencies(sign_genpy sign_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sign_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/sign)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/sign
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(sign_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET sign_generate_messages_cpp)
  add_dependencies(sign_generate_messages_cpp sign_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/sign)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/sign
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(sign_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET sign_generate_messages_eus)
  add_dependencies(sign_generate_messages_eus sign_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/sign)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/sign
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(sign_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET sign_generate_messages_lisp)
  add_dependencies(sign_generate_messages_lisp sign_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/sign)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/sign
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(sign_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET sign_generate_messages_nodejs)
  add_dependencies(sign_generate_messages_nodejs sign_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sign)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sign\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sign
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(sign_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET sign_generate_messages_py)
  add_dependencies(sign_generate_messages_py sign_generate_messages_py)
endif()
