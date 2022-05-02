assembler: assembler.o generateSymbolTable.o generateDataOBJ.o generateCodeOBJ.o lineUtil.o macroLinkList.o symbolLinkList.o macroHandler.o createEntry.o createExtern.o
	gcc -Wall -ansi -pedantic -o assembler assembler.o generateSymbolTable.o generateDataOBJ.o generateCodeOBJ.o lineUtil.o macroLinkList.o symbolLinkList.o macroHandler.o createEntry.o createExtern.o
assembler.o: assembler.c assembler.h  assemblerDefine.h
	gcc -g -Wall -ansi -pedantic -o assembler.o -c assembler.c 
generateSymbolTable.o: generateSymbolTable.c generateSymbolTable.h  assemblerDefine.h symbolLinkList.h lineUtil.h 
	gcc -g -Wall -ansi -pedantic -o generateSymbolTable.o -c generateSymbolTable.c 
generateDataOBJ.o: generateDataOBJ.c generateDataOBJ.h  assemblerDefine.h symbolLinkList.h lineUtil.h
	gcc -g -Wall -ansi -pedantic -o generateDataOBJ.o -c generateDataOBJ.c 
generateCodeOBJ.o: generateCodeOBJ.c generateCodeOBJ.h  assemblerDefine.h symbolLinkList.h lineUtil.h
	gcc -g -Wall -ansi -pedantic -o generateCodeOBJ.o -c generateCodeOBJ.c 
createEntry.o: createEntry.c createEntry.h  assemblerDefine.h symbolLinkList.h lineUtil.h
	gcc -g -Wall -ansi -pedantic -o createEntry.o -c createEntry.c 
createExtern.o: createExtern.c createExtern.h  assemblerDefine.h symbolLinkList.h lineUtil.h
	gcc -g -Wall -ansi -pedantic -o createExtern.o -c createExtern.c 
macroHandler.o: macroHandler.c macroHandler.h  assemblerDefine.h macroLinkList.c 
	gcc -g -Wall -ansi -pedantic -o macroHandler.o -c macroHandler.c 
lineUtil.o: lineUtil.c lineUtil.h commandConst.c assemblerDefine.h commandConstTypeDef.h
	gcc -g -Wall -ansi -pedantic -o lineUtil.o -c lineUtil.c 
macroLinkList.o: macroLinkList.c macroLinkList.h assemblerDefine.h
	gcc -g -Wall -ansi -pedantic -o macroLinkList.o -c macroLinkList.c 
symbolLinkList.o: symbolLinkList.c symbolLinkList.h symbolLinkListTypeDef.h assemblerDefine.h
	gcc -g -Wall -ansi -pedantic -o symbolLinkList.o -c symbolLinkList.c 

clean:
	rm -f *.o assembler *.ob *.ent *.am *.ext

clean_test:
	rm -f  $(file)/*.ob $(file)/*.ent $(file)/*.am $(file)/*.ext

# make -C ../ diff file=bigcode
diff:
	sort $(file)/$(file).ent > tmp
	mv tmp $(file)/$(file).ent
	sort $(file)/$(file).ext > tmp
	mv tmp $(file)/$(file).ext
	diff -b $(file)/$(file).ob $(file)/ref/$(file).ob
	diff -b $(file)/$(file).am $(file)/ref/$(file).am
	diff -b $(file)/$(file).ent $(file)/ref/$(file).ent
	diff -b $(file)/$(file).ext $(file)/ref/$(file).ext
