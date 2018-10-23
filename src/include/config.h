/* src/include/config.h.  Generated from config.h.in by configure.  */
///
/// @file config.h
///
/// @brief Configuração para o aplicativo.
///
/// Gerado automaticamente pelo processo ./configure esse arquivo descreve
/// as configurações de ambiente detectadas automaticamente.
///
/// @author perry.werneck@gmail.com
///
///

#ifndef CONFIG_H_INCLUDED

	#define CONFIG_H_INCLUDED 1

	/// @brief Nome do pacote.
	#define PACKAGE_NAME "pwpet"

	/// @brief Versão atual.
	#define PACKAGE_VERSION "0.1"

 	/// @brief Descrição do aplicativo.
	#define PACKAGE_DESCRIPTION "Protótipo pwPET"

	/// @brief Utiliza a libpam?
/* 	#undef HAVE_PAM */

	/// @brief Tenho ICONV?
	#define HAVE_ICONV 1

	/// @brief Indica se a LIBSSL está disponível.
/*	#undef HAVE_LIBSSL */

	/// @brief Indica se é possível usar traduções
	#define HAVE_LIBINTL 1

	/// @brief Indica se o compilador atual possui suporte a "visibility"
	#define HAVE_GNUC_VISIBILITY 1

	/// @brief Indica se o recurso inotify está disponível no kernel atual.
	#define HAVE_INOTIFY 1

	/// @brief Indica se o pugixml está disponível
/*	#undef HAVE_PUGIXML */

	/// @brief Indica se a plataforma atual suporta a função vasprintf()
	#define HAVE_VASPRINTF 1

	/// @brief Indica se a plataforma atual suporta a função asprintf()
	#define HAVE_ASPRINTF 1

	/// @brief Define to 1 if the system has getopt()
	#define HAVE_GETOPT 1

	/// @brief Define to 1 if the system has localtime_r
	#define HAVE_LOCALTIME_R 1

	/// @brief Define to 1 if the system has gmtime_r
	#define HAVE_GMTIME_R 1

	/// @brief Does the system have systemd?
	#define HAVE_SYSTEMD 1

	/// @brief Defined on most unixes
	#define HAVE_UNISTD_H 1

	/// @brief Tem suporte para a resolução de nomes nova?
	#define HAVE_GETADDRINFO 1

	/// @brief Defined on unix systems
	#define HAVE_UNISTD_H 1

	/// @brief Is kernel sendfile available?
	#define HAVE_SENDFILE 1

	/// @brief Define if poll is available in <poll.h>.
	#define HAVE_POLL 1

	/// @brief Define to 1 if you have the `eventfd' function.
	#define HAVE_EVENTFD 1

#endif // CONFIG_H_INCLUDED

