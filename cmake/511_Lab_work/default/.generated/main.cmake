include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(511_Lab_work_default_library_list )

# Handle files with suffix s, for group default-XC16
if(511_Lab_work_default_default_XC16_FILE_TYPE_assemble)
add_library(A_511_Lab_work_default_default_XC16_assemble OBJECT ${511_Lab_work_default_default_XC16_FILE_TYPE_assemble})
    A_511_Lab_work_default_default_XC16_assemble_rule(A_511_Lab_work_default_default_XC16_assemble)
    list(APPEND 511_Lab_work_default_library_list "$<TARGET_OBJECTS:A_511_Lab_work_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(511_Lab_work_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(A_511_Lab_work_default_default_XC16_assemblePreproc OBJECT ${511_Lab_work_default_default_XC16_FILE_TYPE_assemblePreproc})
    A_511_Lab_work_default_default_XC16_assemblePreproc_rule(A_511_Lab_work_default_default_XC16_assemblePreproc)
    list(APPEND 511_Lab_work_default_library_list "$<TARGET_OBJECTS:A_511_Lab_work_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(511_Lab_work_default_default_XC16_FILE_TYPE_compile)
add_library(A_511_Lab_work_default_default_XC16_compile OBJECT ${511_Lab_work_default_default_XC16_FILE_TYPE_compile})
    A_511_Lab_work_default_default_XC16_compile_rule(A_511_Lab_work_default_default_XC16_compile)
    list(APPEND 511_Lab_work_default_library_list "$<TARGET_OBJECTS:A_511_Lab_work_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(511_Lab_work_default_default_XC16_FILE_TYPE_dependentObject)
add_library(A_511_Lab_work_default_default_XC16_dependentObject OBJECT ${511_Lab_work_default_default_XC16_FILE_TYPE_dependentObject})
    A_511_Lab_work_default_default_XC16_dependentObject_rule(A_511_Lab_work_default_default_XC16_dependentObject)
    list(APPEND 511_Lab_work_default_library_list "$<TARGET_OBJECTS:A_511_Lab_work_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(511_Lab_work_default_default_XC16_FILE_TYPE_bin2hex)
add_library(A_511_Lab_work_default_default_XC16_bin2hex OBJECT ${511_Lab_work_default_default_XC16_FILE_TYPE_bin2hex})
    A_511_Lab_work_default_default_XC16_bin2hex_rule(A_511_Lab_work_default_default_XC16_bin2hex)
    list(APPEND 511_Lab_work_default_library_list "$<TARGET_OBJECTS:A_511_Lab_work_default_default_XC16_bin2hex>")

endif()

# Handle files with suffix elf, for group default-XC16
if(511_Lab_work_default_default_XC16_FILE_TYPE_objcopy_lss)
add_library(A_511_Lab_work_default_default_XC16_objcopy_lss OBJECT ${511_Lab_work_default_default_XC16_FILE_TYPE_objcopy_lss})
    A_511_Lab_work_default_default_XC16_objcopy_lss_rule(A_511_Lab_work_default_default_XC16_objcopy_lss)
    list(APPEND 511_Lab_work_default_library_list "$<TARGET_OBJECTS:A_511_Lab_work_default_default_XC16_objcopy_lss>")

endif()


# Main target for this project
add_executable(511_Lab_work_default_image_oW2QlG9r ${511_Lab_work_default_library_list})

set_target_properties(511_Lab_work_default_image_oW2QlG9r PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${511_Lab_work_default_output_dir}")
target_link_libraries(511_Lab_work_default_image_oW2QlG9r PRIVATE ${511_Lab_work_default_default_XC16_FILE_TYPE_link})
# Add the link options from the rule file.
A_511_Lab_work_default_link_rule( 511_Lab_work_default_image_oW2QlG9r)

# Call bin2hex function from the rule file
A_511_Lab_work_default_bin2hex_rule(511_Lab_work_default_image_oW2QlG9r)

#Add objcopy steps
A_511_Lab_work_default_objcopy_lss_rule(511_Lab_work_default_image_oW2QlG9r)

