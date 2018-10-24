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

  	for(auto property = node.child("property"); property; property = property.next_sibling("property")) {
		contents.push_back(Content::create(property));
  	}
 }

 PDFImporter::Parser::~Parser() {

	for(auto filter : filters) {
		delete filter;
	}

	for(auto content : contents) {
		delete content;
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
	for(auto content : contents) {
		content->reset();
		content->set(document);
	}


 	return true;
 }



