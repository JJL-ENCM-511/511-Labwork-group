set(DEPENDENT_MP_BIN2HEXLab1_default_F5s8550i "/opt/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFLab1_default_F5s8550i ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Lab1/default.elf)
set(DEPENDENT_TARGET_DIRLab1_default_F5s8550i ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Lab1)
set(DEPENDENT_BYPRODUCTSLab1_default_F5s8550i ${DEPENDENT_TARGET_DIRLab1_default_F5s8550i}/${sourceFileNameLab1_default_F5s8550i}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRLab1_default_F5s8550i}/${sourceFileNameLab1_default_F5s8550i}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXLab1_default_F5s8550i} ${DEPENDENT_DEPENDENT_TARGET_ELFLab1_default_F5s8550i} --image ${sourceFileNameLab1_default_F5s8550i} ${addressLab1_default_F5s8550i} ${modeLab1_default_F5s8550i} -mdfp=/home/jyap/.mchp_packs/Microchip/PIC24F-GA-GB_DFP/1.11.479/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRLab1_default_F5s8550i}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFLab1_default_F5s8550i})
add_custom_target(
    dependent_produced_source_artifactLab1_default_F5s8550i 
    DEPENDS ${DEPENDENT_TARGET_DIRLab1_default_F5s8550i}/${sourceFileNameLab1_default_F5s8550i}.s
    )
