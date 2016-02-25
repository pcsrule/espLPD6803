# Component makefile for extras/espLPD6803

# expected anyone using lpd6803 driver includes it as 'lpd6803/lpd6803.h'
INC_DIRS += $(espLPD6803_ROOT)..

# args for passing into compile rule generation
espLPD6803_INC_DIR =
espLPD6803_SRC_DIR = $(espLPD6803_ROOT)

$(eval $(call component_compile_rules,espLPD6803))
