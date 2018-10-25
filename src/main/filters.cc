/**
 * @file src/main/filters.cc
 *
 * @brief Implementa filtros de conteúdo.
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <pdf2db.h>
 #include <regex>

 using namespace std;

/*---[ Implement ]----------------------------------------------------------------------------------*/


 PDFImporter::Filter * PDFImporter::Filter::create(const XMLNode &node) {

 	/// @brief Filtro para selecionar por todo o conteúdo de uma linha dentro de uma página
 	class LineFilter : public PDFImporter::Filter {
	private:
		regex			filter;
		unsigned int	page;
		unsigned int	line;

	public:
		LineFilter(const XMLNode &node) {

			page = node.attribute("page").as_uint(1);
			line = node.attribute("line").as_uint(1);

			filter = node.attribute("value").as_string();

		}

		virtual ~LineFilter() {
		}

		bool test(const Document &document) override {
			return regex_match(document.get(page,line),filter);
		}

 	};


	auto type = node.attribute("type").as_string("text");

	if(!strcasecmp(type,"text")) {
		return new LineFilter(node);
	}

 	/*
 	switch(string(node.attribute("type").as_string("text")).select("text","regex",nullptr)) {
	case 0:
		return new LineFilter(node);

	case 1:
		break;
 	}
 	*/

 	throw EINVAL;

 }



