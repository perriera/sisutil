// YUError.hpp - error class capable of being an idl [out] parameter
// $Id: YUError.hpp

#ifndef _YUERROR_H
#define _YUERROR_H 1

#ifndef _YU_ERROR_H_INCLUDED
#include <sisutil/Error.hpp>
#define _YU_ERROR_H_INCLUDED
#endif

#include <string>

class YUError {
public:
	inline  YUError();
	YUError(const YUError& e);
	virtual ~YUError();					// if non-virtual, do not derive
	inline YUError(const Error& e);

	YUError& operator = (const YUError& e);
	inline                    operator const Error& () const;
	int                       isSuccess() const;
	Error::SeverityLevel      severity() const;
	virtual const std::string text() const;
	Error::ErrorStatus        value() const;
	inline unsigned long      facility() const;
	friend std::ostream& operator << (std::ostream& os, const YUError& e);
	std::ostream& displayOn(std::ostream& os) const;

private:
	class YUError_REP {
	private:
		class NoError : public Error { // to support default ctors
		public:
			inline NoError() : Error(OK) {} // default to OK
			inline NoError(const NoError& rhs) : Error(rhs) {}
			inline virtual ~NoError() {}
			inline NoError& operator = (const NoError& rhs) {
				if (this != &rhs) Error::operator = (rhs);
				return *this;
			}
			virtual Error* clone() const;
			virtual const std::string text() const;
		};
	public:
		YUError_REP(const Error* e);   // removed inline (OJ)
		inline  YUError_REP() : err(new NoError()), count(1) {}
		inline ~YUError_REP() { delete err; }
		Error* err;
		int count;
	};
	YUError_REP* rep;
};

inline YUError::YUError() : rep(NULL) {
	rep = new YUError_REP;
}

YUError::YUError(const Error& e) : rep(NULL) {
	rep = new YUError_REP(&e);
}

inline YUError::operator const Error& () const {
	return *rep->err;
}

inline unsigned long
YUError::facility() const { return operator const Error & ().facility(); }

inline std::ostream&
operator << (std::ostream& os, const YUError& e) { return e.displayOn(os); }

#endif
