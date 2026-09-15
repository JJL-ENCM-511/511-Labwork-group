# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "C:/Users/prudh/.mchp_packs" CACHE PATH "Path to the root of a pack repository.")

function(A_511_Lab_work_default_default_XC16_assemble_rule target)
    set(options
        "-g"
        "-mcpu=24FJ256GA702"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-g,--no-relax")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target} PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h")
endfunction()
function(A_511_Lab_work_default_default_XC16_assemblePreproc_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "-g"
        "-mcpu=24FJ256GA702"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-g,--no-relax")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target} PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h")
endfunction()
function(A_511_Lab_work_default_default_XC16_compile_rule target)
    set(options
        "-g"
        "-mcpu=24FJ256GA702"
        "-O0"
        "-msmart-io=1"
        "-Wall"
        "-msfr-warn=off")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target} PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h")
endfunction()
function(A_511_Lab_work_default_dependentObject_rule target)
    set(options
        "-c"
        "-mcpu=24FJ256GA702")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
endfunction()
function(A_511_Lab_work_default_link_rule target)
    set(options
        "-g"
        "-mcpu=24FJ256GA702"
        "-Wl,--script=p24FJ256GA702.gld,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,memoryfile.xml")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=__DEBUG"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target} PRIVATE "c:/Program Files/Microchip/xc16/v2.10/support/PIC24F/h")
endfunction()
function(A_511_Lab_work_default_bin2hex_rule target)
    add_custom_target(
        511_Lab_work_default_Bin2Hex ALL
        COMMAND ${MP_BIN2HEX} ${511_Lab_work_default_image_name} -a
        WORKING_DIRECTORY ${511_Lab_work_default_output_dir}
        BYPRODUCTS "${511_Lab_work_default_output_dir}/${511_Lab_work_default_image_base_name}.hex"
        COMMENT "Convert build file to .hex")
    add_dependencies(511_Lab_work_default_Bin2Hex ${target})
endfunction()
function(A_511_Lab_work_default_objcopy_lss_rule target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${OBJDUMP}
        ARGS --disassemble --wide --demangle --line-numbers --section-headers --source ${511_Lab_work_default_image_name} > ${511_Lab_work_default_image_base_name}.lss
        WORKING_DIRECTORY ${511_Lab_work_default_output_dir})
endfunction()
