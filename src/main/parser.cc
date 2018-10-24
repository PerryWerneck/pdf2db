/**
 * @file pdfimporter/src/main/parser.cc
 *
 * @brief
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <pdfimporter.h>

/*---[ Implement ]----------------------------------------------------------------------------------*/

 PDFImporter::Parser::Parser(const pugi::xml_node &node) {

  	for(auto filter = node.child("filter"); filter; filter = filter.next_sibling("filter")) {
		filters.push_back(Filter::create(filter));
  	}

 }

 PDFImporter::Parser::~Parser() {

	for(auto filter : filters) {
		delete filter;
	}

 }

 /// @brief Faz o parse do documento.
 bool PDFImporter::Parser::set(const Document &document) {

	// Verifica se o documento atende aos filtros.
 	for(auto filter : filters) {

		if(!filter->test(document)) {
			return false;
		}

 	}

 	// Atendeu aos filtros, extrai conteúdo.


 	return true;
 }



