/**
 * @file pdfimporter/src/main/filters.cc
 *
 * @brief Implementa filtros de conteúdo.
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <pdfimporter.h>


/*---[ Implement ]----------------------------------------------------------------------------------*/


 PDFImporter::Filter * PDFImporter::Filter::create(const XMLNode &node) {

 	/// @brief Filtro para selecionar por todo o conteúdo de uma linha dentro de uma página
 	class LineFilter : public PDFImporter::Filter {
	private:
		string text;
		unsigned int page;
		unsigned int line;

	public:
		LineFilter(const XMLNode &node) {

			page = node.attribute("page").as_uint(1);
			line = node.attribute("line").as_uint(1);
			text = node.attribute("value").as_string();

		}

		virtual ~LineFilter() {
		}

		bool test(const Document &document) override {
			return strcasecmp(document.get(page,line),text.c_str()) == 0;
		}

 	};

 	switch(string(node.attribute("type").as_string("text")).select("text",nullptr)) {
	case 0:
		return new LineFilter(node);

 	}

 	throw EINVAL;

 }



