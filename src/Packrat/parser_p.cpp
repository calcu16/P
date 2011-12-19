Parser::PParser = NULL;
static const Parser& Parser::getPParser()
{
	if(PParser == NULL)
		PParser = new Parser(
			"LBRACK", "\\{{sep}",
			"RBRACK", "\\}{sep}",
			"SEMICOLON", ";{sep}",
			"ASSIGN", "={sep}",
			"RETURN", "return{sep}",
			"ccomment", "/\\*(!\\*/{1})*\\*/",
			"space", "[ \t\r\n\v]",
			"sep", "({comment}|{space})*",
			"newline", "<type=newline><value:\r\n|\n\r|\n|\r>",
			"any", "<type=any><value:{1}>",
			"escape", "<type=escape>\\<value:{newline}|{any}>",
			"ident", "<type=ident><value:![0-9]([a-zA-Z0-9_]*_)>{sep}",
			"o_int", "<type=o_int><value:[0-7]+_>",
			"d_int", "<type=d_int>!0<value:[0-9]+_]",
			"x_int", "<type=x_int>0[xX]<value:[0-9a-fA-F]+_>",
			"int_literal", "<type=int_literal><value:{o_int}|{d_int}|{x_int}>{sep}",
			"char_literal", "<type=char_literal>'<value:{escape}|{any}>'",
			"constant", "<type=constant><value:{int}|{char}>",
			"atom", "<type=atom><value:{ident}|{constant}>",
			"assignment", "{ident}{ASSIGN}{expression}",
			"expression", "<value=expression><value:{atom}|{assignment}"
			"ret_statement", "<type=return>RETURN<value:{expression}>",
			"maybe_dec", 
			"program", "<type=program><value:(cdec|func)*>",
		);
	
}