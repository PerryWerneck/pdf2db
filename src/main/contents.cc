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
 #include <regex>

/*---[ Implement ]----------------------------------------------------------------------------------*/

 PDFImporter::Content::Content(const XMLNode &node) {
 	this->name = node.attribute("name").as_string();
 }

 /// @brief Cria conteúdo de acordo com a descrição XML.
 PDFImporter::Content * PDFImporter::Content::create(const XMLNode &node) {

	/// @brief Filtra linhas de texto.
	class RegexDelimitedBlock : public Content {
	private:
		std::regex	from;
		std::regex	to;
		unsigned int page;

	protected:

		/// @brief Método chamado quando encontra as linhas a filtrar.
		virtual void onTextLine(const char *line) = 0;

		RegexDelimitedBlock(const XMLNode &node) : Content(node) {
			from = node.attribute("begin").as_string();
			to = node.attribute("end").as_string();
			page = node.attribute("page").as_uint(1);
		}

		virtual ~RegexDelimitedBlock() {
		}

		/// @brief Extrai conteúdo do documento.
		bool set(const Document &document) override {

			bool loading = false;

			document.forEach([&loading,this](const char *line) {

				if(loading) {

					if(regex_match(line,to)) {
						loading = false;
						debug("%s: Achei final (%s)",getName().c_str(),string(line).strip().c_str());
						return false;
					}

					debug("[%s]",line);
					onTextLine(line);

				} else if(regex_match(line,from)) {
					debug("%s: Achei inicio (%s)",getName().c_str(),string(line).strip().c_str());
					loading = true;
				}

				return true;
			});

			return true;
		}

	};

	/// @brief Elemento para extrair um bloco de texto.
	class TextBlockContent : public RegexDelimitedBlock {
	private:
		string text;

	protected:

		void onTextLine(const char *line) override {
			if(!text.empty()) {
				text += "\n";
			}
			text += line;
		}

	public:
		TextBlockContent(const XMLNode &node) : RegexDelimitedBlock(node) {
		}

		virtual ~TextBlockContent() {
		}

		void reset() override {
			text.clear();
		}

		bool set(const Document &document) override {
			RegexDelimitedBlock::set(document);
			text.strip();
			debug("%s=\n%s",getName().c_str(),text.c_str());
			return !text.empty();
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



