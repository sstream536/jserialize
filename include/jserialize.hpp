/* Author:                                                  */
/*    karllen                        			    */
/* Date:                                                    */
/*    03/18/2019  First Create                              */
/* Reference:                                               */
/*    from avplayer static_json rapidjson                   */
/* Information:                                             */
/* json serialize deserialize used c++11  		    */
/*----------------------------------------------------------*/

#pragma once
#ifndef _JSON_SERIALIZE_H_
#define _JSON_SERIALIZE_H_

#include <utility>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace jserialize
{
	template<typename T>
	class JPair : public std::pair<const char *, T *>
	{
	public:
		explicit JPair(const char *name, T &t) :
			std::pair<const char *, T *>(name, std::addressof(t))
		{
		}

		const char* name() const
		{
			return this->first;
		}

		T &value() const
		{
			return *(this->second);
		}

		const T &const_value() const
		{
			return *(this->second);
		}
	};

	class JPairMaker
	{
	public:
		JPairMaker() = default;
		static JPairMaker *Instance()
		{
			static JPairMaker _instance;
			return &_instance;
		}

		template<typename T>
		const JPair<T> MakeJPair(const char *name, T &t)
		{
			return JPair<T>(name, t);
		}
	
	private:
		JPairMaker(JPairMaker &) = delete;
		JPairMaker &operator=(JPairMaker &) = delete;
	};

	
	class JAllocator
	{
	public:
		JAllocator() = default;
		static JAllocator *Instance()
		{
			static JAllocator _instance;
			return &_instance;
		}
		
		rapidjson::Document::AllocatorType&
		DocAllocator()
		{
			if (!_document)
			{
				_document.reset(new rapidjson::Document{ rapidjson::kObjectType });
			}
			return _document->GetAllocator();
		}

	private:
		JAllocator(JAllocator &) = delete;
		JAllocator &operator=(JAllocator &) = delete;

	private:
		std::unique_ptr<rapidjson::Document> _document = nullptr;
	};

	template<typename SerializeType, class T>
	inline void JSerializePrivate(SerializeType &serialize, T &obj)
	{
		Serialize(serialize, obj);
	}

	class JSerializeFlow
	{
	public:
		explicit JSerializeFlow(rapidjson::Value &json)
			: _json(json), _allocator(JAllocator::Instance()->DocAllocator())
		{
		}

		template <typename T>
		JSerializeFlow &operator<<(const JPair<T> &jPair)
		{
			SaveVal(jPair.name(), jPair.value());
			return *this;
		}

		template <typename T>
		JSerializeFlow &operator<<(const T &value)
		{
			return operator<<(const_cast<T&>(value));
		}

		template <typename T>
		JSerializeFlow &operator&(const T &v)
		{
			return operator<<(v);
		}

	protected:
		JSerializeFlow &operator<<(int32_t value)
		{
			_json.SetInt(value);
			return *this;
		}

		JSerializeFlow &operator<<(uint32_t value)
		{
			_json.SetUint(value);
			return *this;
		}

		JSerializeFlow &operator<<(int64_t value)
		{
			_json.SetInt64(value);
			return *this;
		}

		JSerializeFlow &operator<<(uint64_t value)
		{
			_json.SetUint64(value);
			return *this;
		}

		JSerializeFlow &operator<<(float value)
		{
			_json.SetFloat(value);
			return *this;
		}

		JSerializeFlow &operator<<(double value)
		{
			_json.SetDouble(value);
			return *this;
		}

		JSerializeFlow &operator<<(bool value)
		{
			_json.SetBool(value);
			return *this;
		}

		JSerializeFlow &operator<<(std::string &value)
		{
			_json.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.size()));
			return *this;
		}

		template<typename T>
		JSerializeFlow &operator<<(std::vector<T> &value)
		{
			_json.SetArray();
			for (auto &v : value)
			{
				rapidjson::Value arrayVal;
				JSerializeFlow serialize(arrayVal);
				serialize << v;
				_json.PushBack(arrayVal, _allocator);
			}
			return *this;
		}

		template<typename T>
		JSerializeFlow &operator<<(std::list<T> &value)
		{
			_json.SetArray();
			for (auto &v : value)
			{
				rapidjson::Value arrayVal;
				JSerializeFlow serialize(arrayVal);
				serialize << v;
				_json.PushBack(arrayVal, _allocator);
			}
			return *this;
		}

		template <typename T>
		JSerializeFlow &operator<<(T& value)
		{
			_json.SetObject();
			SaveVal(value);
			return *this;
		}

		template <typename T>
		void SaveVal(T &data)
		{
			JSerializePrivate(*this, data);
		}

		void SaveVal(const char *name, int32_t data)
		{
			rapidjson::Value value;
			JSerializeFlow ja(value);
			ja << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		void SaveVal(const char *name, uint32_t data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		void SaveVal(const char *name, int64_t data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		void SaveVal(const char *name, uint64_t data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		void SaveVal(const char *name, float data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		void SaveVal(const char *name, double data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		void SaveVal(const char *name, bool data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		void SaveVal(const char *name, std::string &data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		template<typename T>
		void SaveVal(const char *name, std::vector<T> &data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		template<typename T>
		void SaveVal(const char *name, std::list<T> &data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

		template <typename T>
		void SaveVal(const char *name, T &data)
		{
			rapidjson::Value value;
			JSerializeFlow serialize(value);
			serialize << data;
			_json.AddMember(rapidjson::StringRef(name), value, _allocator);
		}

	private:
		JSerializeFlow(JSerializeFlow &) = delete;
		JSerializeFlow &operator =(JSerializeFlow &) = delete;

	private:
		rapidjson::Value &_json;
		rapidjson::Document::AllocatorType &_allocator;
	};

	class JDeSerializeFlow
	{
	public:
		explicit JDeSerializeFlow(const rapidjson::Value &json)
			: _json(json), _allocator(JAllocator::Instance()->DocAllocator())
		{
		}

		template <typename T>
		JDeSerializeFlow &operator>>(const JPair<T> &jPair)
		{
			LoadVal(jPair.name(), jPair.value());
			return *this;
		}

		template <typename T>
		JDeSerializeFlow &operator>>(const T &value)
		{
			return operator>>(const_cast<T&>(value));
		}

		template <typename T>
		JDeSerializeFlow &operator&(const T &v)
		{
			return operator>>(v);
		}
		
		template <typename T>
		JDeSerializeFlow &operator>>(T &value)
		{
			LoadVal(value);
			return *this;
		}

	protected:
		JDeSerializeFlow &operator>>(int32_t &value)
		{
			value = _json.GetInt();
			return *this;
		}

		JDeSerializeFlow &operator>>(uint32_t &value)
		{
			value = _json.GetUint();
			return *this;
		}

		JDeSerializeFlow &operator>>(int64_t &value)
		{
			value = _json.GetInt64();
			return *this;
		}

		JDeSerializeFlow &operator>>(uint64_t &value)
		{
			value = _json.GetUint64();
			return *this;
		}

		JDeSerializeFlow &operator>>(float &value)
		{
			value = _json.GetFloat();
			return *this;
		}

		JDeSerializeFlow &operator>>(double &value)
		{
			value = _json.GetDouble();
			return *this;
		}

		JDeSerializeFlow &operator>>(bool &value)
		{
			value = _json.GetBool();
			return *this;
		}

		JDeSerializeFlow &operator>>(std::string &value)
		{
			value = _json.GetString();
			return *this;
		}

		template<typename T>
		JDeSerializeFlow &operator>>(std::vector<T> &value)
		{
			for (auto &v : _json.GetArray())
			{
				T tmp;
				JDeSerializeFlow deserialize(v);
				deserialize >> tmp;
				value.push_back(tmp);
			}
			return *this;
		}

		template<typename T>
		JDeSerializeFlow &operator>>(std::list<T> &value)
		{
			for (auto &v : _json.GetArray())
			{
				T tmp;
				JDeSerializeFlow deserialize(v);
				deserialize >> tmp;
				value.push_back(tmp);
			}
			return *this;
		}

		template <typename T>
		void LoadVal(T& v)
		{
			JSerializePrivate(*this, v);
		}

		void LoadVal(const char *name, int32_t &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kNumberType)
			{
				return;
			}
			JDeSerializeFlow deserialize(value);
			deserialize >> data;
		}

		void LoadVal(const char *name, uint32_t &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kNumberType)
			{
				return;
			}
			JDeSerializeFlow deserialize(value);
			deserialize >> data;
		}

		void LoadVal(const char *name, int64_t &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kNumberType)
			{
				return;
			}
			JDeSerializeFlow deserialize(value);
			deserialize >> data;
		}

		void LoadVal(const char *name, uint64_t &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kNumberType)
			{
				return;
			}
			JDeSerializeFlow deserialize(value);
			deserialize >> data;
		}

		void LoadVal(const char *name, float &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kNumberType)
			{
				return;
			}
			JDeSerializeFlow deserialize(value);
			deserialize >> data;
		}

		void LoadVal(const char *name, double &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kNumberType)
			{
				return;
			}
			JDeSerializeFlow deserialize(value);
			deserialize >> data;
		}

		void LoadVal(const char *name, bool &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kFalseType &&
				value.GetType() != rapidjson::kTrueType)
			{
				return;
			}
			JDeSerializeFlow deserialize(value);
			deserialize >> data;
		}

		void LoadVal(const char *name, std::string &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kStringType)
			{
				return;
			}
			JDeSerializeFlow deserialize(value);
			deserialize >> data;
		}

		template<typename T>
		void LoadVal(const char *name, std::vector<T> &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kArrayType)
			{
				return;
			}
			for (auto &v : value.GetArray())
			{
				T tmp;
				JDeSerializeFlow deserialize(v);
				deserialize >> tmp;
				data.push_back(tmp);
			}
		}

		template<typename T>
		void LoadVal(const char *name, std::list<T> &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kArrayType)
			{
				return;
			}
			for (auto &v : value.GetArray())
			{
				T tmp;
				JDeSerializeFlow deserialize(v);
				deserialize >> tmp;
				data.push_back(tmp);
			}
		}

		template <typename T>
		void LoadVal(const char *name, T &data)
		{
			if (!_json.HasMember(name))
			{
				return;
			}
			auto &value = _json[name];
			if (value.GetType() != rapidjson::kObjectType)
			{
				return;
			}
			JDeSerializeFlow deserialize(value);
			deserialize >> data;
		}

	private:
		JDeSerializeFlow(JDeSerializeFlow &) = delete;
		JDeSerializeFlow & operator=(JDeSerializeFlow &) = delete;

	private:
		const rapidjson::Value& _json;
		rapidjson::Document::AllocatorType& _allocator;
	};


	//interface
#define GEN_JFLOW_SERIALIZE_FRIEND(ObjName) \
	template<typename SerializeType> \
	friend void Serialize(SerializeType &, ObjName &) 

#define  GEN_JFLOW_SERIALIZE_PAIR(name, objName) \
	JPairMaker::Instance()->MakeJPair(name, objName)

	class JSerialize
	{
	public:
		JSerialize() = default;

		template<typename T>
		static std::string ToString(const T &obj)
		{
			rapidjson::Value value{ rapidjson::kObjectType };
			JSerializeFlow serialize(value);
			serialize << obj;

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			value.Accept(writer);
			return buffer.GetString();
		}

		template<typename T>
		static bool FromString(std::string &json, T &obj)
		{
			rapidjson::Document doc;
			if (doc.Parse(json.data()).HasParseError())
			{
				return false;
			}
			rapidjson::Value value;
			value = doc.Move();

			JDeSerializeFlow deserialize(value);
			deserialize >> obj;

			return true;
		}
	private:
		JSerialize(JSerialize &) = delete;
		JSerialize &operator=(JSerialize &) = delete;
	};
}

#endif //_JSON_SERIALIZE_H_
