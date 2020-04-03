#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <cctype>
#include <cmath>

enum AnalysType // lul c tellement trop genial
{
	PLUS,
	MOINS,
	FOIX,
	DIV,
	LPARENT,
	RPARENT,
	NUM,
	POW
};

std::ostream& operator<<(std::ostream& output, AnalysType value)
{
    switch(value)
    {
    case PLUS:
        output << "PLUS";
        break;
    case MOINS:
        output << "MOINS";
        break;
    case FOIX:
        output << "FOIX";
        break;
    case DIV:
        output << "DIV";
        break;
    case LPARENT:
        output << "LPARENT";
        break;
    case RPARENT:
        output << "RPARENT";
        break;
    case NUM:
        output << "NUM";
        break;
    case POW:
        output << "POW";
        break;
    }

    return output;
}

template <typename T>
std::ostream& operator<<(std::ostream& output, std::vector<T> vect)
{
    for_each(vect.begin(), vect.end(), [&output](T value) -> void
    {
       output << value << "  ";
    });

    return output;

}

bool goodNbrParentesis(std::vector<AnalysType> listType, bool& thereAreOne)
{
	int ouvrantes {0};
    int fermantes {0};
    for(AnalysType caractere: listType)
    {
        if(caractere == LPARENT)
        {
            ++ouvrantes;
            thereAreOne = true;
        }
        else if(caractere == RPARENT)
        {
            ++fermantes;
        }

        if(fermantes > ouvrantes)
        {
            // Pas la peine de continuer, l'expression est invalide.
            return false;
        }
    }

    return ouvrantes == fermantes;
}


std::vector<AnalysType>::iterator right_parentesis_to_cut(std::vector<AnalysType>& a)
{
	int nbrOpenNonClose = 0;

	for (std::vector<AnalysType>::iterator i{ a.begin() }; i != a.end(); i++)
	{
		switch (*i)
		{
		case LPARENT:
			nbrOpenNonClose++;
			break;
		case RPARENT:
			nbrOpenNonClose--;
			break;
		default :
		    break;
		}
		
		if (*i == RPARENT && nbrOpenNonClose == 0)
		{
			return i;
		}
	}

	return a.end();

}

template <typename T>
std::vector<T> insert(typename std::vector<T>::iterator ItFirstPart1, typename std::vector<T>::iterator ItSecondPart1, T const& value,
                      typename std::vector<T>::iterator ItFirstPart2, typename std::vector<T>::iterator ItSecondPart2)
{
    std::vector<T> BigVect {ItFirstPart1, ItSecondPart1};
    
    BigVect.push_back(value);
    
    for(typename std::vector<T>::iterator it{ItFirstPart2} ; it != ItSecondPart2 ; it++)
    {
        BigVect.push_back(*it);
    }

    return BigVect;
}

double process(std::vector<double>& listNbr, std::vector<AnalysType>& listType);
//For using that later

void analysIfParentesis(std::vector<double>& listNbr, std::vector<AnalysType>& listType)
{
    bool thereAreOneP{ false };

	if (!goodNbrParentesis(listType, thereAreOneP))
	{
		throw std::runtime_error("Why is god sending me someone who don't know how to count parentesis ?\nI really hate my life.");
	}
	
	while (thereAreOneP)
	{
		std::vector<AnalysType> to_send{ std::find(listType.begin(), listType.end(), LPARENT) + 1,
										 right_parentesis_to_cut(listType)};

		int nbr_indice{ static_cast<int>(std::count(std::begin(listType), std::find(listType.begin(), listType.end(), LPARENT), NUM) )};

		int nbrBack_indice{ static_cast<int>( std::count(listType.begin(), right_parentesis_to_cut(listType)++, NUM))};

		std::vector<double> nbr_to_send { listNbr.begin() + nbr_indice, listNbr.begin() + nbrBack_indice};

        double result = process(nbr_to_send, to_send);

        listNbr = insert(listNbr.begin(), listNbr.begin() + nbr_indice, result,
                         listNbr.begin() + nbrBack_indice, listNbr.end());

        listType = insert(listType.begin(), std::find(listType.begin(), listType.end(), LPARENT), NUM,
                          right_parentesis_to_cut(listType) + 1, listType.end());
        
        thereAreOneP = false;                  
        goodNbrParentesis(listType, thereAreOneP);
    }
}

double operator_use(double nb1, AnalysType op, double nb2)
{
    switch(op)
    {
    case PLUS:
        return nb1 + nb2;
    case MOINS:
        return nb1 - nb2;
    case FOIX:
        return nb1 * nb2;
    case DIV:
        return nb1 / nb2;
    case POW:
        return pow(nb1, nb2);
    default:
        throw std::runtime_error("It seems the creator of this beautiful calculator don't use correctly his own program.");
    }
}

void calcul(std::vector<double>& listNbr, std::vector<AnalysType>& listType, std::vector<AnalysType>::iterator it)
{
    if( *(it - 1) != NUM || *(it + 1) != NUM )
    {
        throw std::runtime_error("Syntax error.");
    }
    
	int nbr_indice{ static_cast<int>(std::count(std::begin(listType), it, NUM) ) - 1};

    listNbr = insert(listNbr.begin(), listNbr.begin() + nbr_indice, operator_use(listNbr[nbr_indice], *it, listNbr[nbr_indice + 1]),
                         listNbr.begin() + nbr_indice + 2, listNbr.end());

    listType = insert(listType.begin(), it - 1, NUM,
                          it + 2, listType.end());
}

void analysForOperator(std::vector<double>& listNbr, std::vector<AnalysType>& listType, AnalysType operator_)
{
    for(std::vector<AnalysType>::iterator i{listType.begin()} ; i != listType.end() ; i++)
    {
        i = std::find(i, listType.end(), operator_);

        if(i == listType.end())
        {
            break;
        }
        calcul(listNbr, listType, i);
        i = listType.begin(); //else the iterators don't indinct anything
    }
}

double process(std::vector<double>& listNbr, std::vector<AnalysType>& listType)
{

    analysIfParentesis(listNbr, listType);

	analysForOperator(listNbr, listType, POW);
	
	analysForOperator(listNbr, listType, FOIX);
	analysForOperator(listNbr, listType, DIV);

	analysForOperator(listNbr, listType, PLUS);
	analysForOperator(listNbr, listType, MOINS);
    
    if(listType.size() != 1 || listNbr.size() != 1)
    {
        throw std::runtime_error("It must have an error somewhere... Maybe it's your existence.");
    }
    
    return listNbr[0];
}

bool if_Isdigitandpoint(char a)
{
	if (isdigit(a) || a == '.' || a == 'e')
	{
		return true;
	}
	else
	{
		return false;
	}
}

double analyseEntry(std::string entry)
{
	std::vector<double> listNbr;
	std::vector<AnalysType> listType;

	for (std::string::iterator i{ std::begin(entry) }; i != std::end(entry); )
	{
		if (isdigit(*i) || *i == '.' || *i == 'e')
		{
			std::string::iterator it_first_space{ std::find_if_not(i, std::end(entry), if_Isdigitandpoint ) };

			std::string mot{ i, it_first_space };

			std::string::size_type sz;
			double nbr{ std::stod(mot, &sz) };

			listNbr.push_back(nbr);

			listType.push_back(NUM);

			i = it_first_space;
		}
		else if (ispunct(*i) && *i != '.')
		{
			switch (*i)
			{
			case '+':
				listType.push_back(PLUS);
				break;
			case '-':
				listType.push_back(MOINS);
				break;
			case '*':
				listType.push_back(FOIX);
				break;
			case '/':
				listType.push_back(DIV);
				break;
			case '(':
				listType.push_back(LPARENT);
				break;
			case ')':
				listType.push_back(RPARENT);
				break;
			case '^':
				listType.push_back(POW);
				break;
			default:
				throw std::runtime_error("Your operator is just some boolshit. thanks for coming.");
				break;
			}
			i++;
		}
		else if (isspace(*i))
		{
			i = std::find_if_not(i, std::end(entry), isspace);
			//find the first non-space character
		}
		else if (isalpha(*i))
		{
			throw std::runtime_error("DON'T PUT LETTERS. Thanks.");
		}
		else if(*i == '\0')
		{
		    std::cout << "prut" << std::endl;
		    break;
		}
		else
		{
		    throw std::runtime_error("Shit, it's me this time.");
		}
	}



	return process(listNbr, listType);
}

int main()
{
	bool continuer{ true };


	while (continuer)
	{
		std::string entry;

		std::cout << "> ";

		std::getline(std::cin, entry);

		try
		{
			std::cout << analyseEntry(entry) << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << "error : " << e.what() << std::endl;
		}
	}

	return 0;
}