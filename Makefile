#main makefile
# $Amigan: rsmeter/Makefile,v 1.2 2004/10/27 23:57:11 dcp1990 Exp $
all:
	@echo "Building in src"
	@make -C src
	@echo "Building in util/"
	@make -C util/
clean:
	@echo "Cleaning in src"
	@make clean -C src
	@echo "Cleaning in util/"
	@make clean -C util/
