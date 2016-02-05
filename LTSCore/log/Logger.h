#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <vector>
#include <memory>


/**
 * @brief The Logger is an interface to wrap the different log outputs.
 * In order to add a new logger Logger_t following requirements has to be met:
 *	- If using an inplace construction, the Logger_t is allowed to throw
 * a SVD::QMError in order to determine a successful init. In the case of
 * a failed construction the error msg is forwarded to stderr and the added
 * Logger_t is discarded.
 *  - Logger_t has to provide a public method with the following function
 * signiture:
 *  "void Logger_t::Write(const std::string& rMsg) noexcept". As already noted
 * in the signiture the logger is not allowed to throw any exception. In case of
 * an error forward the exception msg to stderr. The Write method should only
 * take care of streaming the given rMsg to the desired destination. The format
 * of the msg handled by the SVD::Logger::Log method. The thread safety is
 * defined in SVD::Logger::Model::Write().
 *
 * For a more detailed implementation specification see Logger::Concept and
 * Logger::Model.
 *
 * NOTE: The SVD::Logger can not be copied, but moving is allowed.
 *
 * @see SVD::Logger::Concept
 * @see SVD::Logger::Model
 * @class Logger Logger.h "Logger/Logger.h"
 */
class Logger
{
	private:
		/**
		 * @brief Concept of each logger implementation. Only the write
		 * functionality needs to be implemented, since the output need a uniform
		 * format.
		 *
		 * @class Concept Logger.h "Logger/Logger.h"
		 */
		class Concept
		{
			public:
				/**
				 * @brief Pure virtual method, that writes the given message to the
				 * desired destination or output.
				 *
				 * @fn Write
				 * @param rMsg std:string, that need to be written.
				 */
				virtual void Write(const std::string& rMsg) noexcept = 0;
				/**
				 * @brief Makes the implementation to an inheritance.
				 *
				 * @fn ~Concept
				 */
				virtual ~Concept() = default;
		};

		/**
		 * @brief The SVD::Model speficies the implementation of th write function.
		 * and uses a std::mutex to synchronize the IO.
		 *
		 * @class Model Logger.h "Logger/Logger.h"
		 */
		template <class Logger_t>
		class Model : public Concept
		{
			public:
				/// default ctor
				Model() noexcept
					: Concept(), fLogger()
				{}

				/**
				 * @brief Moves a algready created logger into the wrapper.
				 *
				 * @fn Model
				 * @param rLogger
				 */
				explicit Model(Logger_t&& rLogger) noexcept
					: Concept(), fLogger(std::move(rLogger))
				{}

				/**
				 * @brief Construct logger inside wrapper.
				 *
				 * @fn Model
				 * @param rArgs Arguments needed to init given Logger_t
				 */
				template<typename ... Args_t>
				Model(Args_t&& ... rArgs)
					: Concept(), fLogger(std::forward<Args_t&&>(rArgs)...)
				{}

				/**
				 * @brief Thread safe implementation of the Write function.
				 *
				 * @fn Write
				 * @param rMsg Message to log.
				 */
				virtual void Write(const std::string& rMsg) noexcept final
				{
					std::lock_guard<std::mutex> lock(this->fMutex);
					(this->fLogger).Write(rMsg);
				}

				/// default dtor
				virtual ~Model() = default;

			private:
				Logger_t fLogger; /**< Implementation of logger. */
				std::mutex fMutex; /**< Mutex to sychronize IO. */
		};

	public:
		/**
		 * @brief Defines the type of log msg.
		 *
		 * @enum Type_t
		 */
		enum class Type_t : unsigned char {
			WARNING, /**< Describes a recoverable error */
			FATAL, /**< Describes a run breaking event */
			INFO /**< Additional informations */
		};

		/**
		 * @brief ctor, reserving size for loggers
		 *
		 * @fn Logger
		 * @param size Reserve [size] Logger_t inside container.
		 * size = [1, 0xffffffff]
		 */
        explicit Logger() noexcept;

		/// delete copy ctor
		Logger(const Logger&) = delete;

		/// delete copy assignment
		Logger& operator=(const Logger&) = delete;

		/// move ctor
		Logger(Logger&& rOther) noexcept;

		/// move assignment
		Logger& operator=(Logger&& rOther) noexcept;

		/**
		 * @brief Moves the already created logger to the container
		 *
		 * @fn Add
		 * @param rLogger moveable logger implementation.
		 */
		template<class Logger_t>
		void Add(Logger_t&& rLogger) noexcept;

		/**
		 * @brief Add Logger to container
		 *
		 * @throw throws what ctor of Logger_t throws.
		 * @fn Add
		 * @param rArgs arguments needed for init of Logger_t
		 */
		template<class Logger_t, typename ... Args_t>
		void Add(Args_t&& ... rArgs);

		/**
		 * @brief Write the msg to all loggers. with the following format:
		 *  [yyyy-mm-dd hh:mm:ss]::[Who]::[Type]::[Message][new line]
		 *
		 * If a exception occurres the error is forward to stderr.
		 *
		 * @fn Log
		 * @param type Type of log
		 * @param rWho Object issueing a log.
		 * @param rWhat Report
		 */
		void Log(Type_t type, const std::string& rWho,
						 const std::string& rWhat) noexcept;

	private:
		std::vector<std::unique_ptr<Concept>> fLoggers; /**< Logger container */
};


template<class Logger_t>
void Logger::Add(Logger_t&& rLogger) noexcept
{
	(this->fLoggers).emplace_back(
				std::unique_ptr<Concept>(new Model<Logger_t>(std::move(rLogger))));
}

template<class Logger_t, typename ... Args_t>
void Logger::Add(Args_t&& ... rArgs)
{
	auto logger = std::unique_ptr<Concept>( new
				Model<Logger_t>(std::forward<Args_t>(rArgs)...));
	(this->fLoggers).emplace_back(std::move(logger));
}


class GlobalLogger
{
  private:
    GlobalLogger() noexcept
        : fLogger()
    {}

    static std::unique_ptr<GlobalLogger> fgpLogger;
    Logger fLogger;


  public:
    static void Log(Logger::Type_t type, const std::string& rWho,
        const std::string& rWhat) noexcept
    {
        auto& rLogger = GlobalLogger::GetLogger();
        rLogger.Log(type, rWho, rWhat);
    }

    static Logger& GetLogger() noexcept
    {
       if (fgpLogger.get() == nullptr) {
           fgpLogger.reset(new(std::nothrow) GlobalLogger());
       }

       return (fgpLogger)->fLogger;
    }
};

#endif // LOGGER_H
