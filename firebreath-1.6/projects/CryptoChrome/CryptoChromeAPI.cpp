/**********************************************************\

  Browser API for Crypto Chrome.

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"
#include <stdexcept>
#include <iostream>

#include "stlplus-portability/subprocesses.hpp"

#include "CryptoChromeAPI.h"

///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant CryptoChromeAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant CryptoChromeAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn CryptoChromePtr CryptoChromeAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
CryptoChromePtr CryptoChromeAPI::getPlugin()
{
    CryptoChromePtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string CryptoChromeAPI::get_testString()
{
    return m_testString;
}

void CryptoChromeAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string CryptoChromeAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void CryptoChromeAPI::testEvent()
{
    fire_test();
}

std::string CryptoChromeAPI::get_gpg()
{
  if (m_gpgpath.length() == 0) {
    return "gpg";
  }
  return m_gpgpath;
}

// Configuration
std::string CryptoChromeAPI::gpg_version()
{
	stlplus::subprocess p;

	stlplus::arg_vector args("--version");

	p.spawn(get_gpg(), args, false, true, false);

	std::string output;
	p.read_stdout(output);
    
    return output;
}

std::string CryptoChromeAPI::set_gpg_path(std::string path)
{
    m_gpgpath = path;
    return this->gpg_version();
}



// Text Processing
std::string CryptoChromeAPI::decrypt(std::string crypt_txt)
{
	stlplus::subprocess p;

	stlplus::arg_vector args;
	args += "--quiet";
	args += "--no-tty";
	args += "--decrypt";
	args += "--use-agent";
	args += "--logger-fd";
	args += "1";

	p.spawn(get_gpg(), args, true, true, false);
	p.write_stdin(crypt_txt);
	p.close_stdin();

	std::string output;
	p.read_stdout(output);

	return output;
}

std::string CryptoChromeAPI::encrypt(std::string recipient, std::string clear_txt)
{
	stlplus::subprocess p;

	stlplus::arg_vector args;
	args += "--quiet";
	args += "--no-tty";
	args += "--encrypt";
	args += "--always-trust"; // TODO: remove this.
	args += "--armor";
	args += "--logger-fd";
	args += "1";
	args += "--recipient";
	args += recipient;

	p.spawn(get_gpg(), args, true, true, false);
	p.write_stdin(clear_txt);
	p.close_stdin();

	std::string output;
	p.read_stdout(output);

	return output;
}

std::string CryptoChromeAPI::clearsign(std::string clear_txt)
{
	stlplus::subprocess p;

	stlplus::arg_vector args;
	args += "--quiet";
	args += "--no-tty";
	args += "--clearsign";
	args += "--armor";
	args += "--logger-fd";
	args += "1";

	p.spawn(get_gpg(), args, true, true, false);
	p.write_stdin(clear_txt);
	p.close_stdin();

	std::string output;
	p.read_stdout(output);

	return output;
}


std::string CryptoChromeAPI::encrypt_sign(std::string recipient, std::string clear_txt)
{
	stlplus::subprocess p;

	stlplus::arg_vector args;
	args += "--quiet";
	args += "--no-tty";
	args += "--encrypt";
	args += "--sign";
	args += "--always-trust"; // TODO: remove this.
	args += "--armor";
	args += "--logger-fd";
	args += "1";
	args += "--recipient";
	args += recipient;

	p.spawn(get_gpg(), args, true, true, false);
	p.write_stdin(clear_txt);
	p.close_stdin();

	std::string output;
	p.read_stdout(output);

	return output;
}