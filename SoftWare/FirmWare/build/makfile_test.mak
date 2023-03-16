

#Target = $(Target_def)
#Target_def  = AdvancedClock


#$(info HOMEPATH=$(HOMEPATH))
#$(info HOMEDRIVE=$(HOMEDRIVE))

#Dependency := $(Module) 
#Module := $(Dependency)

#Target ?= $(Compiler)


#Compiler = armcc
#Compiler += armclang
#Compiler += $(cc)

#cc = gcc

#DIR += $(C_DIR)
#C_DIR = /123

src_file := 1.c 2.c 3.c

object := $(src_file:%.c=%.o)

A = $(B)

B = C

C = hello 

D = $($(A))

all:
#	@echo $(Target)
#	@echo $(Module)
#	@echo $(Compiler)
#	@echo $(DIR)
	@echo $(object)
	@echo $(D)

