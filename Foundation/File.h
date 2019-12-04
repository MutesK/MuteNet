#pragma once

/*
  성능 이슈로 인해, 파일 포인터로 바꾸는 작업 필수
*/
#include "pch.h"

namespace Util
{

	enum class EFileType
	{
		Read,
		ReadWrite,
		Append,
	};

	enum class EOpenType
	{
		Binary,
		Text,
	};

	template <EFileType FileType = EFileType::ReadWrite, EOpenType OpenType = EOpenType::Binary>
	class File final
	{
	private:
		std::fstream  _fstream;

		std::string _FilePath;

		std::size_t  _Begin = 0;
		std::size_t  _End = 0;

	public:
		File(const std::string& FilePath);
		~File();

		bool Open();
		void Close();

		uint32_t Write(const char* Buffer);
		void ReadLine(char* OUT LineBuffer, const uint32_t Length);
		void ReadToEOF(std::string& OUT Buffer);
		uint32_t FileSize();
	private:
		bool OpenTextStream();
		bool OpenFileStream();
	};

	template<EFileType FileType, EOpenType OpenType>
	inline File<FileType, OpenType>::File(const std::string& FilePath)
		:_FilePath(FilePath)
	{
	}

	template<EFileType FileType, EOpenType OpenType>
	inline File<FileType, OpenType>::~File()
	{
		Close();
	}

	template<EFileType FileType, EOpenType OpenType>
	inline bool File<FileType, OpenType>::Open()
	{
		if (OpenType == EOpenType::Text)
		{
			return OpenTextStream();
		}
		else
		{
			return OpenFileStream();
		}

	}

	template<EFileType FileType, EOpenType OpenType>
	inline void File<FileType, OpenType>::Close()
	{
		if (_fstream.is_open())
		{
			_fstream.close();
		}
	}

	template<EFileType FileType, EOpenType OpenType>
	inline uint32_t File<FileType, OpenType>::Write(const char* Buffer)
	{
		if (Buffer && _fstream.is_open())
		{
			_fstream.write(Buffer, strlen(Buffer));
			_fstream.write("\r\n", strlen("\r\n"));


			_Begin = static_cast<size_t>(_fstream.tellp());

			return _Begin;
		}

		return 0;
	}

	template<EFileType FileType, EOpenType OpenType>
	inline void File<FileType, OpenType>::ReadLine(char* OUT Buffer, const uint32_t Length)
	{
		if (Buffer == nullptr || Length <= 0)
			return;

		if (_fstream.is_open())
		{
			if (_fstream.good())
			{
				_fstream.getline(Buffer, Length);
			}
		}
	}

	template<EFileType FileType, EOpenType OpenType>
	inline void File<FileType, OpenType>::ReadToEOF(std::string& OUT Buffer)
	{
		if (_fstream.is_open())
		{
			_fstream.seekg(0, std::ios::end);
			Buffer.resize(_fstream.tellg());

			_fstream.seekg(0, std::ios::beg);
			_fstream.read(const_cast<char*>(Buffer.c_str()), Buffer.size());
		}
	}

	template<EFileType FileType, EOpenType OpenType>
	inline uint32_t File<FileType, OpenType>::FileSize()
	{
		if (_fstream.is_open())
		{
			_fstream.seekg(0, std::ios::end);
			return static_cast<uint32_t>(_fstream.tellg());
		}

		return 0;
	}

	template<EFileType FileType, EOpenType OpenType>
	inline bool File<FileType, OpenType>::OpenTextStream()
	{
		switch (FileType)
		{
		case EFileType::Read:
		{
			auto openMode = (std::ios::in | std::ios::binary);

			_fstream.open(_FilePath.c_str(), openMode);
			if (!_fstream.is_open())
			{
				throw new std::exception("File is not Open");
			}

			_Begin = static_cast<size_t>(_fstream.tellg());
		}
		break;
		case EFileType::ReadWrite:
		{
			auto openMode = (std::ios::in | std::ios::out | std::ios::trunc);

			_fstream.open(_FilePath.c_str(), openMode);
			if (!_fstream.is_open())
			{
				throw new std::exception("File is not Open");
			}

			_Begin = static_cast<size_t>(_fstream.tellg());
		}
		break;
		case EFileType::Append:
		{
			auto openMode = (std::ios::in | std::ios::out | std::ios::app);

			_fstream.open(_FilePath.c_str(), openMode);
			if (!_fstream.is_open())
			{
				throw new std::exception("File is not Open");
			}

			_Begin = static_cast<size_t>(_fstream.tellg());
		}
		break;

		}

		return true;
	}

	template<EFileType FileType, EOpenType OpenType>
	inline bool File<FileType, OpenType>::OpenFileStream()
	{
		switch (FileType)
		{
		case EFileType::Read:
		{
			auto openMode = (std::ios::in | std::ios::binary);

			_fstream.open(_FilePath.c_str(), openMode);
			if (!_fstream.is_open())
			{
				throw new std::exception("File is not Open");
			}

			_Begin = static_cast<size_t>(_fstream.tellg());
		}
		break;
		case EFileType::ReadWrite:
		{
			auto openMode = (std::ios::out | std::ios::binary);

			_fstream.open(_FilePath.c_str(), openMode);
			if (!_fstream.is_open())
			{
				throw new std::exception("File is not Open");
			}

			_Begin = static_cast<size_t>(_fstream.tellg());
		}
		break;
		case EFileType::Append:
		{
			auto openMode = (std::ios::app | std::ios::out | std::ios::binary);

			_fstream.open(_FilePath.c_str(), openMode);
			if (!_fstream.is_open())
			{
				throw new std::exception("File is not Open");
			}

			_Begin = static_cast<size_t>(_fstream.tellg());
		}
		break;

		}

		return true;
	}

}