include $(ZRT_ROOT)/Makefile.env
NAME=search


run: 
	@sed s@{ABS_PATH}@${CURDIR}/@g $(NAME).manifest.template > $(NAME).manifest
	@cp ../src/search search.nexe
	$(SETARCH) $(ZEROVM_ROOT)/zerovm -M$(NAME).manifest -v4
#	cat stdout.data

