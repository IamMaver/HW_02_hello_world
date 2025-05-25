# Путь к исходникам ядра (может потребоваться изменить)
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
# Текущая директория с исходниками
PWD := $(shell pwd)

# Цель по умолчанию — сборка модулей
all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

# Очистка собранных файлов
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean

format:
	clang-format -i *.c

check:
	./check.sh