#include "cimpMacroParser.hpp"


// Macro* Macro::parseMacroDefiniton(CXCursor cursor, File* file)
// {
	
// 	CXTranslationUnit TU = (CXTranslationUnit)file->getTranslationUnit();
// 	CXSourceRange range = clang_getCursorExtent(cursor);

// 	std::string name, macro;
// 	unsigned  num_tokens;
// 	CXToken*  tokens;
// 	CXCursor* cursors = 0;
// 	clang_tokenize(TU, range, &tokens, &num_tokens);

// 	cursors = (CXCursor *)malloc(num_tokens * sizeof(CXCursor));
// 	clang_annotateTokens(TU, tokens, num_tokens, cursors);

// 	for (unsigned i = 0; i != num_tokens; ++i) {
// 		CXString str = clang_getTokenSpelling(TU, tokens[i]);
		
// 		if(i == 1)
// 			name = clang_getCString(str);
// 		else if(i == 2) 
// 			macro = clang_getCString(str);

// 		clang_disposeString(str);
// 	}

// 	free(cursors);
// 	clang_disposeTokens(TU, tokens, num_tokens);

// 	return new Macro(name, macro);
// }