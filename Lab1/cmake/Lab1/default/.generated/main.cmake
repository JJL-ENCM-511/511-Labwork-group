include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Lab1_default_library_list )

# Handle files with suffix s, for group default-XC16
if(Lab1_default_default_XC16_FILE_TYPE_assemble)
add_library(Lab1_default_default_XC16_assemble OBJECT ${Lab1_default_default_XC16_FILE_TYPE_assemble})
    Lab1_default_default_XC16_assemble_rule(Lab1_default_default_XC16_assemble)
    list(APPEND Lab1_default_library_list "$<TARGET_OBJECTS:Lab1_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(Lab1_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(Lab1_default_default_XC16_assemblePreproc OBJECT ${Lab1_default_default_XC16_FILE_TYPE_assemblePreproc})
    Lab1_default_default_XC16_assemblePreproc_rule(Lab1_default_default_XC16_assemblePreproc)
    list(APPEND Lab1_default_library_list "$<TARGET_OBJECTS:Lab1_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(Lab1_default_default_XC16_FILE_TYPE_compile)
add_library(Lab1_default_default_XC16_compile OBJECT ${Lab1_default_default_XC16_FILE_TYPE_compile})
    Lab1_default_default_XC16_compile_rule(Lab1_default_default_XC16_compile)
    list(APPEND Lab1_default_library_list "$<TARGET_OBJECTS:Lab1_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(Lab1_default_default_XC16_FILE_TYPE_dependentObject)
add_library(Lab1_default_default_XC16_dependentObject OBJECT ${Lab1_default_default_XC16_FILE_TYPE_dependentObject})
    Lab1_default_default_XC16_dependentObject_rule(Lab1_default_default_XC16_dependentObject)
    list(APPEND Lab1_default_library_list "$<TARGET_OBJECTS:Lab1_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(Lab1_default_default_XC16_FILE_TYPE_bin2hex)
add_library(Lab1_default_default_XC16_bin2hex OBJECT ${Lab1_default_default_XC16_FILE_TYPE_bin2hex})
    Lab1_default_default_XC16_bin2hex_rule(Lab1_default_default_XC16_bin2hex)
    list(APPEND Lab1_default_library_list "$<TARGET_OBJECTS:Lab1_default_default_XC16_bin2hex>")

endif()

# Handle files with suffix elf, for group default-XC16
if(Lab1_default_default_XC16_FILE_TYPE_objcopy_lss)
add_library(Lab1_default_default_XC16_objcopy_lss OBJECT ${Lab1_default_default_XC16_FILE_TYPE_objcopy_lss})
    Lab1_default_default_XC16_objcopy_lss_rule(Lab1_default_default_XC16_objcopy_lss)
    list(APPEND Lab1_default_library_list "$<TARGET_OBJECTS:Lab1_default_default_XC16_objcopy_lss>")

endif()


# Main target for this project
add_executable(Lab1_default_image_F5s8550i ${Lab1_default_library_list})

set_target_properties(Lab1_default_image_F5s8550i PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${Lab1_default_output_dir}")
target_link_libraries(Lab1_default_image_F5s8550i PRIVATE ${Lab1_default_default_XC16_FILE_TYPE_link})
# Add the link options from the rule file.
Lab1_default_link_rule( Lab1_default_image_F5s8550i)

# Call bin2hex function from the rule file
Lab1_default_bin2hex_rule(Lab1_default_image_F5s8550i)

#Add objcopy steps
Lab1_default_objcopy_lss_rule(Lab1_default_image_F5s8550i)

