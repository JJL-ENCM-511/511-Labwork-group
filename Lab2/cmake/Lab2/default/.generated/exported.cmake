set(DEPENDENT_MP_BIN2HEXLab2_default__yo7nq5E "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFLab2_default__yo7nq5E ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Lab2/default.elf)
set(DEPENDENT_TARGET_DIRLab2_default__yo7nq5E ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Lab2)
set(DEPENDENT_BYPRODUCTSLab2_default__yo7nq5E ${DEPENDENT_TARGET_DIRLab2_default__yo7nq5E}/${sourceFileNameLab2_default__yo7nq5E}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRLab2_default__yo7nq5E}/${sourceFileNameLab2_default__yo7nq5E}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXLab2_default__yo7nq5E} ${DEPENDENT_DEPENDENT_TARGET_ELFLab2_default__yo7nq5E} --image ${sourceFileNameLab2_default__yo7nq5E} ${addressLab2_default__yo7nq5E} ${modeLab2_default__yo7nq5E} -mdfp=C:/Users/prudh/.mchp_packs/Microchip/PIC24F-GA-GB_DFP/1.11.479/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRLab2_default__yo7nq5E}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFLab2_default__yo7nq5E})
add_custom_target(
    dependent_produced_source_artifactLab2_default__yo7nq5E 
    DEPENDS ${DEPENDENT_TARGET_DIRLab2_default__yo7nq5E}/${sourceFileNameLab2_default__yo7nq5E}.s
    )
