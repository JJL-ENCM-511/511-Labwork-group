set(DEPENDENT_MP_BIN2HEX511_Lab_work_default_oW2QlG9r "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF511_Lab_work_default_oW2QlG9r ${CMAKE_CURRENT_LIST_DIR}/../../../../out/511_Lab_work/default.elf)
set(DEPENDENT_TARGET_DIR511_Lab_work_default_oW2QlG9r ${CMAKE_CURRENT_LIST_DIR}/../../../../out/511_Lab_work)
set(DEPENDENT_BYPRODUCTS511_Lab_work_default_oW2QlG9r ${DEPENDENT_TARGET_DIR511_Lab_work_default_oW2QlG9r}/${sourceFileName511_Lab_work_default_oW2QlG9r}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR511_Lab_work_default_oW2QlG9r}/${sourceFileName511_Lab_work_default_oW2QlG9r}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX511_Lab_work_default_oW2QlG9r} ${DEPENDENT_DEPENDENT_TARGET_ELF511_Lab_work_default_oW2QlG9r} --image ${sourceFileName511_Lab_work_default_oW2QlG9r} ${address511_Lab_work_default_oW2QlG9r} ${mode511_Lab_work_default_oW2QlG9r} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR511_Lab_work_default_oW2QlG9r}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF511_Lab_work_default_oW2QlG9r})
add_custom_target(
    dependent_produced_source_artifact511_Lab_work_default_oW2QlG9r 
    DEPENDS ${DEPENDENT_TARGET_DIR511_Lab_work_default_oW2QlG9r}/${sourceFileName511_Lab_work_default_oW2QlG9r}.s
    )
