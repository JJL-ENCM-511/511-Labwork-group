set(DEPENDENT_MP_BIN2HEXLab2_default_hMbRhy8x "/opt/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFLab2_default_hMbRhy8x ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Lab2/default.elf)
set(DEPENDENT_TARGET_DIRLab2_default_hMbRhy8x ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Lab2)
set(DEPENDENT_BYPRODUCTSLab2_default_hMbRhy8x ${DEPENDENT_TARGET_DIRLab2_default_hMbRhy8x}/${sourceFileNameLab2_default_hMbRhy8x}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRLab2_default_hMbRhy8x}/${sourceFileNameLab2_default_hMbRhy8x}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXLab2_default_hMbRhy8x} ${DEPENDENT_DEPENDENT_TARGET_ELFLab2_default_hMbRhy8x} --image ${sourceFileNameLab2_default_hMbRhy8x} ${addressLab2_default_hMbRhy8x} ${modeLab2_default_hMbRhy8x} -mdfp=/home/jyap/.mchp_packs/Microchip/PIC24F-GA-GB_DFP/1.11.479/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRLab2_default_hMbRhy8x}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFLab2_default_hMbRhy8x})
add_custom_target(
    dependent_produced_source_artifactLab2_default_hMbRhy8x 
    DEPENDS ${DEPENDENT_TARGET_DIRLab2_default_hMbRhy8x}/${sourceFileNameLab2_default_hMbRhy8x}.s
    )
