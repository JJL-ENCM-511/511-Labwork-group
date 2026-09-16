set(DEPENDENT_MP_BIN2HEX511_Lab_work_default__NUk4Iuo "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF511_Lab_work_default__NUk4Iuo ${CMAKE_CURRENT_LIST_DIR}/../../../../out/511_Lab_work/production/default-production.elf)
set(DEPENDENT_TARGET_DIR511_Lab_work_default__NUk4Iuo ${CMAKE_CURRENT_LIST_DIR}/../../../../out/511_Lab_work/production)
set(DEPENDENT_BYPRODUCTS511_Lab_work_default__NUk4Iuo ${DEPENDENT_TARGET_DIR511_Lab_work_default__NUk4Iuo}/${sourceFileName511_Lab_work_default__NUk4Iuo}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR511_Lab_work_default__NUk4Iuo}/${sourceFileName511_Lab_work_default__NUk4Iuo}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX511_Lab_work_default__NUk4Iuo} ${DEPENDENT_DEPENDENT_TARGET_ELF511_Lab_work_default__NUk4Iuo} --image ${sourceFileName511_Lab_work_default__NUk4Iuo} ${address511_Lab_work_default__NUk4Iuo} ${mode511_Lab_work_default__NUk4Iuo} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR511_Lab_work_default__NUk4Iuo}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF511_Lab_work_default__NUk4Iuo})
add_custom_target(
    dependent_produced_source_artifact511_Lab_work_default__NUk4Iuo 
    DEPENDS ${DEPENDENT_TARGET_DIR511_Lab_work_default__NUk4Iuo}/${sourceFileName511_Lab_work_default__NUk4Iuo}.s
    )
