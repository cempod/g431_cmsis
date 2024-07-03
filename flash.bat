openocd -f interface/stlink.cfg -f target/stm32g4x.cfg -c "program build/g431_cmsis.elf verify reset exit"
