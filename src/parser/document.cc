/**
 * @file src/main/parser.cc
 *
 * @brief
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <pdf2db.h>
 #include <cstring>

/*---[ Implement ]----------------------------------------------------------------------------------*/

 PDFImporter::Parser::Document::Document(cppdb::session &sql, const pugi::xml_node &node) : PDFImporter::Parser::Abstract(sql,node) {

  	for(auto filter = node.child("filter"); filter; filter = filter.next_sibling("filter")) {
		filters.push_back(Filter::create(filter));
  	}

 }

 PDFImporter::Parser::Document::~Document() {

	for(auto filter : filters) {
		delete filter;
	}

 }

 /// @brief Faz o parse do documento.
 bool PDFImporter::Parser::Document::set(cppdb::session &sql, const PDFImporter::Document &document) {

	// Verifica se o documento atende aos filtros.
 	for(auto filter : filters) {

		if(!filter->test(document)) {
			return false;
		}

 	}

 	// Atendeu aos filtros.
	return Parser::Abstract::set(sql,document);

 }





