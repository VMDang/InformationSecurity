//.h file code:

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <iostream>
#include <regex>
#include <optional>
#include "stringhelper.h"
#include "stringbuilder.h"

class Decrpty
{

public:
	static void main(std::vector<std::wstring> &args);

	static std::wstring createSplitStringArray(const std::wstring &inputString, const std::wstring &key);

	static std::vector<std::wstring> hexToString(std::vector<std::wstring> &hexStrings);

	static std::wstring hexToString(const std::wstring &hexString);

	static bool isCharASCIIAlphabet(const std::wstring &s);

	static std::wstring generateSpaceString(const std::wstring &s);
};

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <codecvt>

class StringHelper
{
public:
	static std::wstring toLower(std::wstring source)
	{
		std::transform(source.begin(), source.end(), source.begin(), [](unsigned char c){ return std::tolower(c); });
		return source;
	}

	static std::wstring toUpper(std::wstring source)
	{
		std::transform(source.begin(), source.end(), source.begin(), [](unsigned char c){ return std::toupper(c); });
		return source;
	}

	static std::wstring trimStart(std::wstring source, const std::wstring &trimChars = L" \t\n\r\v\f")
	{
		return source.erase(0, source.find_first_not_of(trimChars));
	}

	static std::wstring trimEnd(std::wstring source, const std::wstring &trimChars = L" \t\n\r\v\f")
	{
		return source.erase(source.find_last_not_of(trimChars) + 1);
	}

	static std::wstring trim(std::wstring source, const std::wstring &trimChars = L" \t\n\r\v\f")
	{
		return trimStart(trimEnd(source, trimChars), trimChars);
	}

	static std::wstring replace(std::wstring source, const std::wstring &find, const std::wstring &replace)
	{
		std::size_t pos = 0;
		while ((pos = source.find(find, pos)) != std::wstring::npos)
		{
			source.replace(pos, find.length(), replace);
			pos += replace.length();
		}
		return source;
	}

	static bool startsWith(const std::wstring &source, const std::wstring &value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(0, value.length(), value) == 0;
	}

	static bool endsWith(const std::wstring &source, const std::wstring &value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(source.length() - value.length(), value.length(), value) == 0;
	}

	static std::vector<std::wstring> split(const std::wstring &source, wchar_t delimiter)
	{
		std::vector<std::wstring> output;
		std::wistringstream ss(source);
		std::wstring nextItem;

		while (std::getline(ss, nextItem, delimiter))
		{
			output.push_back(nextItem);
		}

		return output;
	}

	template<typename T>
	static std::wstring toString(const T &subject)
	{
		std::wostringstream ss;
		ss << subject;
		return ss.str();
	}

	template<typename T>
	static T fromString(const std::wstring &subject)
	{
		std::wistringstream ss(subject);
		T target;
		ss >> target;
		return target;
	}

	static bool isEmptyOrWhiteSpace(const std::wstring &source)
	{
		if (source.length() == 0)
			return true;
		else
		{
			for (std::size_t index = 0; index < source.length(); index++)
			{
				if (!std::isspace(source[index]))
					return false;
			}

			return true;
		}
	}

	template<typename T>
	static std::wstring formatSimple(const std::wstring &input, T arg)
	{
		std::wostringstream ss;
		std::size_t lastFormatChar = std::wstring::npos;
		std::size_t percent = std::wstring::npos;
		while ((percent = input.find(L'%', percent + 1)) != std::wstring::npos)
		{
			if (percent + 1 < input.length())
			{
				if (input[percent + 1] == L'%')
				{
					percent++;
					continue;
				}

				std::size_t formatEnd = std::wstring::npos;
				std::wstring index;
				for (std::size_t i = percent + 1; i < input.length(); i++)
				{
					if (input[i] == L's')
					{
						index = L"1";
						formatEnd = i;
						break;
					}
					else if (input[i] == L'$' && i + 1 < input.length() && input[i + 1] == L's')
					{
						index = input.substr(percent + 1, i - percent - 1);
						formatEnd = i + 1;
						break;
					}
					else if (!std::isdigit(input[i]))
						break;					
				}

				if (formatEnd != std::wstring::npos)
				{
					ss << input.substr(lastFormatChar + 1, percent - lastFormatChar - 1);
					lastFormatChar = formatEnd;

					if (index == L"1")
						ss << arg;
					else
						throw std::runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (lastFormatChar + 1 < input.length())
			ss << input.substr(lastFormatChar + 1);

		return ss.str();
	}

	template<typename T>
	static std::wstring formatSimple(const std::wstring &input, const std::vector<T> &args)
	{
		std::wostringstream ss;
		std::size_t lastFormatChar = std::wstring::npos;
		std::size_t percent = std::wstring::npos;
		while ((percent = input.find(L'%', percent + 1)) != std::wstring::npos)
		{
			if (percent + 1 < input.length())
			{
				if (input[percent + 1] == L'%')
				{
					percent++;
					continue;
				}

				std::size_t formatEnd = std::wstring::npos;
				std::wstring index;
				for (std::size_t i = percent + 1; i < input.length(); i++)
				{
					if (input[i] == L's')
					{
						index = L"1";
						formatEnd = i;
						break;
					}
					else if (input[i] == L'$' && i + 1 < input.length() && input[i + 1] == L's')
					{
						index = input.substr(percent + 1, i - percent - 1);
						formatEnd = i + 1;
						break;
					}
					else if (!std::isdigit(input[i]))
						break;					
				}

				if (formatEnd != std::wstring::npos)
				{
					ss << input.substr(lastFormatChar + 1, percent - lastFormatChar - 1);
					lastFormatChar = formatEnd;
					ss << args[std::stoi(index) - 1];
				}
			}
		}

		if (lastFormatChar + 1 < input.length())
			ss << input.substr(lastFormatChar + 1);

		return ss.str();
	}

	template<typename T1, typename T2>
	static std::wstring formatSimple(const std::wstring &input, T1 arg1, T2 arg2)
	{
		std::wostringstream ss;
		std::size_t lastFormatChar = std::wstring::npos;
		std::size_t percent = std::wstring::npos;
		while ((percent = input.find(L'%', percent + 1)) != std::wstring::npos)
		{
			if (percent + 1 < input.length())
			{
				if (input[percent + 1] == L'%')
				{
					percent++;
					continue;
				}

				std::size_t formatEnd = std::wstring::npos;
				std::wstring index;
				for (std::size_t i = percent + 1; i < input.length(); i++)
				{
					if (input[i] == L's')
					{
						index = L"1";
						formatEnd = i;
						break;
					}
					else if (input[i] == L'$' && i + 1 < input.length() && input[i + 1] == L's')
					{
						index = input.substr(percent + 1, i - percent - 1);
						formatEnd = i + 1;
						break;
					}
					else if (!std::isdigit(input[i]))
						break;					
				}

				if (formatEnd != std::wstring::npos)
				{
					ss << input.substr(lastFormatChar + 1, percent - lastFormatChar - 1);
					lastFormatChar = formatEnd;

					if (index == L"1")
						ss << arg1;
					else if (index == L"2")
						ss << arg2;
					else
						throw std::runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (lastFormatChar + 1 < input.length())
			ss << input.substr(lastFormatChar + 1);

		return ss.str();
	}

	template<typename T1, typename T2, typename T3>
	static std::wstring formatSimple(const std::wstring &input, T1 arg1, T2 arg2, T3 arg3)
	{
		std::wostringstream ss;
		std::size_t lastFormatChar = std::wstring::npos;
		std::size_t percent = std::wstring::npos;
		while ((percent = input.find(L'%', percent + 1)) != std::wstring::npos)
		{
			if (percent + 1 < input.length())
			{
				if (input[percent + 1] == L'%')
				{
					percent++;
					continue;
				}

				std::size_t formatEnd = std::wstring::npos;
				std::wstring index;
				for (std::size_t i = percent + 1; i < input.length(); i++)
				{
					if (input[i] == L's')
					{
						index = L"1";
						formatEnd = i;
						break;
					}
					else if (input[i] == L'$' && i + 1 < input.length() && input[i + 1] == L's')
					{
						index = input.substr(percent + 1, i - percent - 1);
						formatEnd = i + 1;
						break;
					}
					else if (!std::isdigit(input[i]))
						break;					
				}

				if (formatEnd != std::wstring::npos)
				{
					ss << input.substr(lastFormatChar + 1, percent - lastFormatChar - 1);
					lastFormatChar = formatEnd;

					if (index == L"1")
						ss << arg1;
					else if (index == L"2")
						ss << arg2;
					else if (index == L"3")
						ss << arg3;
					else
						throw std::runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (lastFormatChar + 1 < input.length())
			ss << input.substr(lastFormatChar + 1);

		return ss.str();
	}

	static std::string wstring_to_string(const std::wstring& wstr)
	{
		return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(wstr);
	}
};
#include <string>
#include <sstream>

class StringBuilder
{
private:
	std::wstring privateString;

public:
	StringBuilder()
	{
	}

	StringBuilder(const std::wstring &initialString)
	{
		privateString = initialString;
	}

	StringBuilder(std::size_t capacity)
	{
		ensureCapacity(capacity);
	}

	wchar_t charAt(std::size_t index)
	{
		return privateString[index];
	}

	StringBuilder *append(const std::wstring &toAppend)
	{
		privateString += toAppend;
		return this;
	}

	template<typename T>
	StringBuilder *append(const T &toAppend)
	{
		privateString += toString(toAppend);
		return this;
	}

	StringBuilder *insert(std::size_t position, const std::wstring &toInsert)
	{
		privateString.insert(position, toInsert);
		return this;
	}

	template<typename T>
	StringBuilder *insert(std::size_t position, const T &toInsert)
	{
		privateString.insert(position, toString(toInsert));
		return this;
	}

	std::wstring toString()
	{
		return privateString;
	}

	std::size_t length()
	{
		return privateString.length();
	}

	void setLength(std::size_t newLength)
	{
		privateString.resize(newLength);
	}

	std::size_t capacity()
	{
		return privateString.capacity();
	}

	void ensureCapacity(std::size_t minimumCapacity)
	{
		privateString.reserve(minimumCapacity);
	}

	StringBuilder *remove(std::size_t start, std::size_t end)
	{
		privateString.erase(start, end - start);
		return this;
	}

	StringBuilder *replace(std::size_t start, std::size_t end, const std::wstring &newString)
	{
		privateString.replace(start, end - start, newString);
		return this;
	}

private:
	template<typename T>
	static std::wstring toString(const T &subject)
	{
		std::wostringstream ss;
		ss << subject;
		return ss.str();
	}
};

#include <string>
#include <vector>

int main(int argc, char **argv)
{
    std::vector<std::wstring> args(argv + 1, argv + argc);
    Decrpty::main(args);
}

#include "snippet.h"

void Decrpty::main(std::vector<std::wstring> &args)
{

	std::vector<std::wstring> arrayOfHexMessages(11);
	arrayOfHexMessages[0] = L"1D1C4554250A111200244E04001C1100490B0B413F540E0844030005560C015941541C591E19041A164916504B1116000E19154F170711050445154F105500480F55161A43130A50191C0C06001B09001618040D10521A1E49174F440E0113";
	arrayOfHexMessages[1] = L"0C1A4306181B111A4F0F000E0145151C4544151303430A1553421D0756001C1A4F10064E1454004E0945060341130A00061B5253100D0D4D084504411B5515534A011C54480817454D1C171B0017004E07110F1A17";
	arrayOfHexMessages[2] = L"1D1C4506044B0401454154101D45150D5001164103464F034E01001806111B164E540E4C141B130710481850530D024D0A010049064E00030A170A50161C1B4E4A141D10000000590018061C521D0C00161A021C1D5001194F1A";
	arrayOfHexMessages[3] = L"1D1C4554370202164E045202520608044801174105534F07000F17151E0A16594F124F451D17131714541C1E47540E4C1F1D1342001A0C0E49111658165516594A00001D4E0653414D06061A49111C001C12410A0D461315521101544F36134515131B450D0D03";
	arrayOfHexMessages[4] = L"1A00521B0F0C4517490649131309410749030B0018551D03534213131345131700111C53161A1507054C550245051A491D101F450B1A450B06175353071601520F55000D5315164D1E";
	arrayOfHexMessages[5] = L"0C02451A411F0D1600124D061E090407544415041E5300080001130F56061A184E130A00071C044E074F000253114F4F09550648004E03181D100145";
	arrayOfHexMessages[6] = L"1D1C4554220A0000411300041B1509115244040D1F4F4F0D4E0D050F5604015941541C481A12154E0749051845064F491C551D4E004E0A0B49111B4542061D4D1A1916075441154F1F1810484F124F451D17131714541C1F4E";
	arrayOfHexMessages[7] = L"1B2761540818451E410545471D0341004801450802491B0F410E520D1311061C52074F4F1554150601000605521A0E4D0A06524F034E370207452149141007544A34171D00321B41001C1148411A0B003F110E00055211506110034502141C";
	arrayOfHexMessages[8] = L"1D1C455411190A074F024F0B52120007000A040C09444F0746161713562E170B42111D4F0054071C0B4D553752110A4B4F180B540D0109020E1C53540A1054460F071C17490E06534D010B1A45114F481615050B0000120541060B000B1A15000914492D0F0016";
	arrayOfHexMessages[9] = L"1B114D110C09000100154F471E0A0E1F001115410D544F12480752120204000A00150144531A0E1A44441A074E540E544F0C1D55174E03080C1153410C11544E0F03160600061A56085516180003005218";

	arrayOfHexMessages[10] = L"0B184F170A4B061A50094515014515154B0145004C4E1A0B420700411903521B49001C00121A054E014E160259041B001B1D174D450F164D084500490C1218454A001D1D5441124E095513094410064E145415060100051C411D01540A0D0600151D4911060507";

	std::wstring secretMessageHex = L"0B184F170A4B061A50094515014515154B0145004C4E1A0B420700411903521B49001C00121A054E014E160259041B001B1D174D450F164D084500490C1218454A001D1D5441124E095513094410064E145415060100051C411D01540A0D0600151D4911060507";
	std::wstring secretMessage = hexToString(secretMessageHex);
	std::vector<std::wstring> arrayOfASCIIEncMessages = hexToString(arrayOfHexMessages); // converting all the hex to ASCII
	StringXORer *stringXORer = new StringXORer();
	std::unordered_map<int, std::wstring> finalKeyNonHex;
	Set<int> *knownKeyPositiosn = std::set<int>();
	std::vector<std::wstring> finalKey(100);



	for (int i = 0; i < arrayOfASCIIEncMessages.size(); i++)
	{
		std::unordered_map<int, int> counter = std::unordered_map();
		std::vector<int> knownSpaceIndex = std::vector();

		for (int j = 0; j < arrayOfASCIIEncMessages.size(); j++)
		{
			if (!(i == j))
			{ //we cannot be testing a cipher against itself

				std::wstring xorResult = stringXORer->encode(arrayOfASCIIEncMessages[i],arrayOfASCIIEncMessages[j]);

				for (int k = 0; k < xorResult.length();k++)
				{

					//now we check if the result is an ASCII character
					if (isCharASCIIAlphabet(StringHelper::toString(xorResult[k])))
					{
						if (counter.find(k) == counter.end())
						{
							counter.emplace(k, 1);
						}
						else
						{
							counter.emplace(k,counter[k] + 1);
						}
					}

				}
			}
		}

		std::unordered_map<int, int>::const_iterator it = counter.begin();
		while (it != counter.end())
		{
			Map::Entry *pair = (Map::Entry)*it;
			//if the count is likely above 6 then we can say this is probably the right character for the key
			if (static_cast<std::optional<int>>(pair->getValue()) > 6)
			{
				knownSpaceIndex.push_back(static_cast<std::optional<int>>(pair->getKey()));
			}
			it++;
		}

		//if we xor string with spaces we can then output the key
		std::wstring xorWithSpaces = stringXORer->encode(arrayOfASCIIEncMessages[i],generateSpaceString(arrayOfASCIIEncMessages[i]));
		for (auto l : knownSpaceIndex)
		{
			finalKeyNonHex.emplace(l,StringHelper::toString(xorWithSpaces[l]));
			finalKey[l] = StringHelper::toString(xorWithSpaces[l]);
			knownKeyPositiosn->add(l);

		}
	}

	StringBuilder *sb1 = new StringBuilder();
	StringBuilder *sb2 = new StringBuilder();
	int s = 0;

	for (int m = 0; m < finalKey.size(); m++)
	{
		if (finalKey[m] != L"")
		{
			sb1->append(finalKey[m]);
			sb2->append(finalKey[m]);
		}
		else
		{
			sb1->append(L"*");
			sb2->append(static_cast<wchar_t>(s));
		}
	}

	std::wstring manualKeyGuess = L"it takes a great deal of bravery to stand up to our enemies but just as much to stand up to our friends";

	StringHelper::replace(manualKeyGuess, L"*",StringHelper::toString(static_cast<wchar_t>(s)));

	std::wcout << L"key is : " << sb1->toString() << std::endl;
	for (int i = 0; i < arrayOfASCIIEncMessages.size(); i++)
	{
		std::wcout << L"Message " << i << L" is: " << createSplitStringArray(arrayOfASCIIEncMessages[i],manualKeyGuess) << std::endl;
	}



	//System.out.println("message is : " + createSplitStringArray(arrayOfASCIIEncMessages[4],manualKeyGuess));



	delete sb2;
	delete sb1;
	delete knownKeyPositiosn;
	delete stringXORer;
}

std::wstring Decrpty::createSplitStringArray(const std::wstring &inputString, const std::wstring &key)
{
	int kLength = key.length();
	int sLength = inputString.length();
	bool exact = false;
	std::vector<std::wstring> splitStrings;
	std::vector<std::wstring> splitStringsDecoded;
	StringXORer *xoRer = new StringXORer();
	StringBuilder *sb = new StringBuilder();

	if (sLength % kLength == 0)
	{
		splitStrings = std::vector<std::wstring>(sLength / kLength);
		splitStringsDecoded = std::vector<std::wstring>(sLength / kLength);
	}
	else
	{
		splitStrings = std::vector<std::wstring>((sLength / kLength) + 1);
		splitStringsDecoded = std::vector<std::wstring>((sLength / kLength) + 1);

	}

	for (int i = 0; i < splitStrings.size(); i++)
	{
		if ((i * kLength) + kLength - 1 < inputString.length())
		{
			splitStrings[i] = inputString.substr(i * kLength, ((i * kLength) + kLength) - (i * kLength));
		}
		else
		{
			splitStrings[i] = inputString.substr(i * kLength, (inputString.length() - 1) - (i * kLength));
		}

	}

	for (int j = 0; j < splitStringsDecoded.size(); j++)
	{

		splitStringsDecoded[j] = xoRer->encode(splitStrings[j],key);
	}

	for (auto s : splitStringsDecoded)
	{
		sb->append(s);
	}


	delete sb;
	delete xoRer;
	return sb->toString();
}

std::vector<std::wstring> Decrpty::hexToString(std::vector<std::wstring> &hexStrings)
{
	StringXORer *stringXORer = new StringXORer();
	std::vector<std::wstring> asciiiString(hexStrings.size());

	for (int i = 0; i < hexStrings.size(); i++)
	{
		asciiiString[i] = stringXORer->convertHexToString(hexStrings[i]);
	}

	delete stringXORer;
	return asciiiString;
}

std::wstring Decrpty::hexToString(const std::wstring &hexString)
{
	StringXORer *stringXORer = new StringXORer();

	delete stringXORer;
	return stringXORer->convertHexToString(hexString);
}