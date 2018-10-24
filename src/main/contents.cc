/**
 * @file src/main/contents.cc
 *
 * @brief Implementa extratores de conteúdo.
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <pdfimporter.h>


/*---[ Implement ]----------------------------------------------------------------------------------*/

 PDFImporter::Content::Content(const XMLNode &node) {
 	this->name = node.attribute("name").as_string();
 }

 /// @brief Cria conteúdo de acordo com a descrição XML.
 PDFImporter::Content * PDFImporter::Content::create(const XMLNode &node) {

	/// @brief Elemento para extrair um bloco de texto.
	class TextBlockContent : public Content {
	private:
		string from;
		string to;
		unsigned int page;
		string text;

	protected:

		virtual void onTextLine(const char *line) {
			if(!text.empty()) {
				text += "\n";
			}
			text += line;
		}

	public:
		TextBlockContent(const XMLNode &node) : Content(node) {
			from = node.attribute("begin").as_string();
			to = node.attribute("end").as_string();
			page = node.attribute("page").as_uint(1);
		}

		virtual ~TextBlockContent() {
		}

		void reset() override {
			text.clear();
		}

		/// @brief Extrai conteúdo do documento.
		bool set(const Document &document) override {

			bool loading = false;

			document.forEach([&loading,this](const char *line) {

				if(loading) {

					if(string(line).hasPrefix(to.c_str())) {
						return false;
					}

					debug("[%s]",line);
					onTextLine(line);

				} else if(string(line).hasPrefix(from.c_str())) {
					loading = true;
				}

				return true;
			});

			text.strip();

			debug("Achei \"%s\":\n%s\n",getName().c_str(),text.c_str());
			return true;
		}

	};

 	switch(string(node.attribute("type").as_string("text-block")).select("text-block","regex",nullptr)) {
	case 0:
		return new TextBlockContent(node);

	case 1:
		break;
 	}

 	throw EINVAL;

 }



