#include "SharedBufferOptionParser.h"

#include "OptionPrinter.h"

#include <boost/filesystem.hpp>

namespace options = boost::program_options;

int SharedBufferOptionParser::parseArguments(int argc, char **argv)
{
    try {
        const std::string &appName = boost::filesystem::basename(argv[0]);
        options::options_description desc("Options");
        desc.add_options()
                ("help", u8"- напечатать это сообщение")
                ("key,k", options::value<std::string>()->required(), u8"- платформо-независимый ключ для доступа к объекту разделяемой памяти")
                ("buffer_count,c", options::value<BufferId>()->required(), u8"- количество буферов")
                ("buffer_size,s", options::value<BufferSize>()->required(), u8"- глубина каждого буфера");

        options::positional_options_description positionalOptions;
        options::variables_map vm;

        try {
            options::store(options::command_line_parser(argc, argv).options(desc).run(), vm);
            if (vm.count("help")) {
                std::cout << u8"Сервер буферов сигналов. Доступ к буферам организован через системную разделяемую память." << std::endl << std::endl;
                OptionPrinter::printStandardAppDesc(appName, std::cout, desc, &positionalOptions);
                return ERROR_IN_COMMAND_LINE;
            }

            options::notify(vm);
        } catch (boost::program_options::required_option &e) {
            OptionPrinter::formatRequiredOptionError(e);
            std::cerr << u8"ОШИБКА: " << e.what() << std::endl << std::endl;
            OptionPrinter::printStandardAppDesc(appName, std::cout, desc, &positionalOptions);
            return ERROR_IN_COMMAND_LINE;
        } catch (boost::program_options::error &e) {
            std::cerr << u8"ОШИБКА: " << e.what() << std::endl << std::endl;
            OptionPrinter::printStandardAppDesc(appName, std::cout, desc, &positionalOptions);
            return ERROR_IN_COMMAND_LINE;
        }

        key = QString::fromStdString(vm["key"].as<std::string>());
        bufferCount = vm["buffer_count"].as<BufferId>();
        bufferSize = vm["buffer_size"].as<BufferSize>();
        return SUCCESS;
    } catch (std::exception &e) {
        std::cerr << "Unhandled Exception reached the top of main: " << e.what() << std::endl
                  << "Application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;
    }
}

QString SharedBufferOptionParser::getKey() const
{
    return key;
}

BufferId SharedBufferOptionParser::getBufferCount() const
{
    return bufferCount;
}

BufferSize SharedBufferOptionParser::getBufferSize() const
{
    return bufferSize;
}
