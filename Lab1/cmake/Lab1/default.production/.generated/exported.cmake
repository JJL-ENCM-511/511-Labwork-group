set(DEPENDENT_MP_BIN2HEXLab1_default_HXqjAnHn "/opt/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFLab1_default_HXqjAnHn ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Lab1/production/default-production.elf)
set(DEPENDENT_TARGET_DIRLab1_default_HXqjAnHn ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Lab1/production)
set(DEPENDENT_BYPRODUCTSLab1_default_HXqjAnHn ${DEPENDENT_TARGET_DIRLab1_default_HXqjAnHn}/${sourceFileNameLab1_default_HXqjAnHn}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRLab1_default_HXqjAnHn}/${sourceFileNameLab1_default_HXqjAnHn}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXLab1_default_HXqjAnHn} ${DEPENDENT_DEPENDENT_TARGET_ELFLab1_default_HXqjAnHn} --image ${sourceFileNameLab1_default_HXqjAnHn} ${addressLab1_default_HXqjAnHn} ${modeLab1_default_HXqjAnHn} -mdfp=/home/jyap/.mchp_packs/Microchip/PIC24F-GA-GB_DFP/1.11.479/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRLab1_default_HXqjAnHn}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFLab1_default_HXqjAnHn})
add_custom_target(
    dependent_produced_source_artifactLab1_default_HXqjAnHn 
    DEPENDS ${DEPENDENT_TARGET_DIRLab1_default_HXqjAnHn}/${sourceFileNameLab1_default_HXqjAnHn}.s
    )
