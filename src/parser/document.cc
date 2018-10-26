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

 	try {

		// Extrai conteúdo.
		for(auto property : properties) {

			std::vector<string> text;

			document.get(property->getPage(),property->getLine(),text);

			property->clear();
			property->set(text);

		}

		// Atualiza banco de dados.
		cppdb::transaction guard(sql);
		store(sql);
		guard.commit();

 	} catch(const std::exception &e) {

		std::cerr << e.what() << std::endl;
		return false;

 	} catch(...) {

		std::cerr << "Unexpected error" << std::endl;
		return false;

 	}


 	return true;
 }





