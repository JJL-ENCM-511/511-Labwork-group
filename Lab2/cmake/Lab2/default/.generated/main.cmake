include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Lab2_default_library_list )

# Handle files with suffix s, for group default-XC16
if(Lab2_default_default_XC16_FILE_TYPE_assemble)
add_library(Lab2_default_default_XC16_assemble OBJECT ${Lab2_default_default_XC16_FILE_TYPE_assemble})
    Lab2_default_default_XC16_assemble_rule(Lab2_default_default_XC16_assemble)
    list(APPEND Lab2_default_library_list "$<TARGET_OBJECTS:Lab2_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(Lab2_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(Lab2_default_default_XC16_assemblePreproc OBJECT ${Lab2_default_default_XC16_FILE_TYPE_assemblePreproc})
    Lab2_default_default_XC16_assemblePreproc_rule(Lab2_default_default_XC16_assemblePreproc)
    list(APPEND Lab2_default_library_list "$<TARGET_OBJECTS:Lab2_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(Lab2_default_default_XC16_FILE_TYPE_compile)
add_library(Lab2_default_default_XC16_compile OBJECT ${Lab2_default_default_XC16_FILE_TYPE_compile})
    Lab2_default_default_XC16_compile_rule(Lab2_default_default_XC16_compile)
    list(APPEND Lab2_default_library_list "$<TARGET_OBJECTS:Lab2_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(Lab2_default_default_XC16_FILE_TYPE_dependentObject)
add_library(Lab2_default_default_XC16_dependentObject OBJECT ${Lab2_default_default_XC16_FILE_TYPE_dependentObject})
    Lab2_default_default_XC16_dependentObject_rule(Lab2_default_default_XC16_dependentObject)
    list(APPEND Lab2_default_library_list "$<TARGET_OBJECTS:Lab2_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(Lab2_default_default_XC16_FILE_TYPE_bin2hex)
add_library(Lab2_default_default_XC16_bin2hex OBJECT ${Lab2_default_default_XC16_FILE_TYPE_bin2hex})
    Lab2_default_default_XC16_bin2hex_rule(Lab2_default_default_XC16_bin2hex)
    list(APPEND Lab2_default_library_list "$<TARGET_OBJECTS:Lab2_default_default_XC16_bin2hex>")

endif()

# Handle files with suffix elf, for group default-XC16
if(Lab2_default_default_XC16_FILE_TYPE_objcopy_lss)
add_library(Lab2_default_default_XC16_objcopy_lss OBJECT ${Lab2_default_default_XC16_FILE_TYPE_objcopy_lss})
    Lab2_default_default_XC16_objcopy_lss_rule(Lab2_default_default_XC16_objcopy_lss)
    list(APPEND Lab2_default_library_list "$<TARGET_OBJECTS:Lab2_default_default_XC16_objcopy_lss>")

endif()


# Main target for this project
add_executable(Lab2_default_image_hMbRhy8x ${Lab2_default_library_list})

set_target_properties(Lab2_default_image_hMbRhy8x PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${Lab2_default_output_dir}")
target_link_libraries(Lab2_default_image_hMbRhy8x PRIVATE ${Lab2_default_default_XC16_FILE_TYPE_link})
# Add the link options from the rule file.
Lab2_default_link_rule( Lab2_default_image_hMbRhy8x)

# Call bin2hex function from the rule file
Lab2_default_bin2hex_rule(Lab2_default_image_hMbRhy8x)

#Add objcopy steps
Lab2_default_objcopy_lss_rule(Lab2_default_image_hMbRhy8x)

