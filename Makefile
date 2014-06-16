OBJ				=	obj
BIN				=	bin
VPATH			=	src
CXXFLAGS		=	-I	"./include"	-I	"./API/include"
PROGRAM_NAME	=	Alarm
CHK_DIR_EXIST	=	if not exist
MKDIR         	=	mkdir
REN				=	ren


CPPFILES	=	$(wildcard	$(VPATH)/*.c)
OBJFILES	=	$(addprefix	$(OBJ)/,$(notdir	$(CPPFILES:.c=.o)))

all: createdir $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJFILES)
	$(CXX)	$(OBJFILES)	-o	$(BIN)/$@
	
$(OBJ)/%.o: $(VPATH)/%.c
	$(CXX)	-c	$(CXXFLAGS)	$<	-o	$@
	
	
clean:
	del $(OBJ)\*.o
	del $(BIN)\*.exe
	rmdir $(OBJ)
	rmdir $(BIN)
	

#bin ve obj dosyaları varsa bile yeniden oluşturmaz yoksa oluşturur..
#kontrolü @$(CHK_DIR_EXIST) yani "if not exist" ile ve peşinden dosya yolu yada dosya ismi verilerek kontrol edilir..
createdir:
	@$(CHK_DIR_EXIST)	$(OBJ)	$(MKDIR)	$(OBJ)
	@$(CHK_DIR_EXIST)	$(BIN)	$(MKDIR)	$(BIN)