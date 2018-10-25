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

 #include <pdf2db.h>
 #include <regex>

/*---[ Implement ]----------------------------------------------------------------------------------*/

 PDFImporter::Content::Content(const XMLNode &node) {
 	this->name = node.attribute("name").as_string();
	this->page = node.attribute("page").as_uint(1);
 }

 /// @brief Cria conteúdo de acordo com a descrição XML.
 PDFImporter::Content * PDFImporter::Content::create(const XMLNode &node) {

	/// @brief Filtra linhas de texto.
	class RegexDelimitedBlock : public Content {
	private:
		std::regex	from;
		std::regex	to;

	protected:

		/// @brief Método chamado quando encontra as linhas a filtrar.
		virtual void onTextLine(const char *line) = 0;

		RegexDelimitedBlock(const XMLNode &node) : Content(node) {
			from = node.attribute("begin").as_string();
			to = node.attribute("end").as_string();
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
//						debug("%s: Achei final (%s)",getName().c_str(),string(line).strip().c_str());
						return false;
					}

//					debug("[%s]",line);
					onTextLine(line);

				} else if(regex_match(line,from)) {
//					debug("%s: Achei inicio (%s)",getName().c_str(),string(line).strip().c_str());
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
//			text.strip();
//			debug("%s=\n%s",getName().c_str(),text.c_str());
			return !text.empty();
		}
	};

	/// @brief Elemento para extrair um valor
	class RegexExtractedValue : public Content {
	private:
		std::regex expression;
		unsigned int line;

		/// @brief Método chamado quando encontra as linhas a filtrar.
		virtual void onExtractedResult(size_t index, const char *result) {
//			debug("%s(%u)=\"%s\"",getName().c_str(),(unsigned int) index, result);
		}


	public:
		RegexExtractedValue(const XMLNode &node) : Content(node) {
			expression = node.attribute("expression").as_string();
			line = node.attribute("line").as_uint(0);
		}

		virtual ~RegexExtractedValue() {
		}

		void reset() override {
		}

		bool set(const Document &document) override {

			if(line) {

				// Tem linha pesquiso só ela.
				std::smatch match;

				if(std::regex_search(document.getString(page,line), match, this->expression)) {
					for(size_t ix = 1; ix < match.size();ix++) {
//						onExtractedResult(ix-1,string(match.str(1).c_str()).strip().c_str());
					}
				}

			} else {

				// Não tem linha, pesquiso a página toda.
				document.forEach([this](const char *line) {

					std::smatch match;
					string str = line;

					if(std::regex_search(str, match, this->expression)) {
						for(size_t ix = 1; ix < match.size();ix++) {
//							onExtractedResult(ix-1,string(match.str(1).c_str()).strip().c_str());
						}
						return false;
					}

					return true;

				});

			}

			return true;
		}
	};

	/*
 	switch(string(node.attribute("type").as_string("text-block")).select("text-block","regex",nullptr)) {
	case 0:
		return new TextBlockContent(node);

	case 1:
		return new RegexExtractedValue(node);

 	}
 	*/

 	throw EINVAL;

 }



