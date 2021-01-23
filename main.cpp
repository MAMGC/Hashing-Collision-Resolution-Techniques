#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int search_steps_method1 =0 ;
int search_steps_method2 =0 ;
int search_steps_method3 =0 ;

int search_1 =0 ;
int search_2 =0 ;
int search_3 =0 ;

int deleted_1 =0 ;
int deleted_2 =0 ;
int deleted_3 =0 ;

int actual_size_1 =0;
int actual_size_2 =0;
int actual_size_3 =0;

int Entered_hash_file_1=0;
int Entered_hash_file_2=0;
int Entered_hash_file_3=0;

int Entered_hash_array_1=0 ;
int Entered_hash_array_2=0;
int Entered_hash_array_3 =0 ;


struct student
{
    char name [51] ;
    char gpa [4] ;
    char address [51];
    char mobile [12] ;
    char department[3];

};

struct student3
{
    char name [51] ;
    char gpa [4] ;
    char address [51];
    char mobile [12] ;
    char department[3];
    int offset ;
    int next ;
};

struct PIndex
{
    char name [51];
    int offset;
};

istream& operator>>(istream &input, student &s)
{
    cout << "Enter name , GPA , address , mobile , Department \n";
    input.getline(s.name, 51);
    input.getline(s.gpa ,4)  ;
    input.getline(s.address , 51);
    input.getline(s.mobile , 12) ;
    input.getline(s.department , 3);

    return input;
}

ostream & operator<<(ostream& out, student & s)
{
    out<<"Name : "<< s.name << endl;
    out<<"GPA  : "<< s.gpa << endl;
    out<<"Address : "<< s.address << endl;
    out<<"Mobile : "<< s.mobile << endl;
    out<<"Department : "<< s.department << endl;
    out <<"------------------------------------\n";
    return out;
}


istream& operator>>(istream &input, student3 &s)
{
    cout << "Enter name , GPA , address , mobile , Department \n";
    input.getline(s.name, 51);
    input.getline(s.gpa ,4)  ;
    input.getline(s.address , 51);
    input.getline(s.mobile , 12) ;
    input.getline(s.department , 3);

    return input;
}

ostream & operator<<(ostream& out, student3 & s)
{
    out<<"Name : "<< s.name << endl;
    out<<"GPA  : "<< s.gpa << endl;
    out<<"Address : "<< s.address << endl;
    out<<"Mobile : "<< s.mobile << endl;
    out<<"Department : "<< s.department << endl;
    out<<"offset : " <<s.offset <<endl ;
    out <<"Next : " << s.next <<endl ;
    out<<"------------------------------------\n";
    return out;
}

int Hash_1 ( const char *key , int table_size )
{
    int result = key[0] * key [1];
    return result%table_size;
}

int Hash_2 ( const char *key , int table_size)
{
    int result = key[2] * key [3];
    return result%table_size;
}

class method1
{
    PIndex hashed_index [101][3];
    fstream dataFile;

public:

    method1 (char* dFile)
    {
        dataFile.open(dFile,ios::in|ios::out);
        for (int i=0 ; i<101 ; i++)
        {
            for (int j=0 ; j<3 ; j++)
            {

                strcpy( hashed_index [i][j].name , "///" );
                hashed_index [i][j].offset = -1 ;
            }
        }
    }

    void add_student (student & s )
    {

        dataFile.seekp(0,ios::end);
        int of = dataFile.tellp();
        dataFile.write( (char*) & s , sizeof (student));

        PIndex key ;
        strcpy(key.name , s.name);
        key.offset = of;

        cout << "\n********** Adding student ***********\n\n";
        cout << "key = " << key.name << "  , offset = " << key.offset <<endl ;


        int index = Hash_1 ( key.name , 101);

        cout << "index of hash function = " << index <<endl;

        insert_Hashed_index (index , key);


    }

    void insert_Hashed_index (int idx , PIndex key  )
    {
        bool inserted = false ;
        int num = idx ;

        while ( num < 101 && inserted == false)
        {
            for (int k=0 ; k<3 ; k++)
            {
                if ( strcmp( hashed_index[num][k].name , "///" ) == 0 || strcmp(hashed_index[num][k].name ,"***" )==0 ) // check if the cell is empty or contains deleted item
                {
                    strcpy(this->hashed_index[num][k].name , key.name );
                    this->hashed_index[num][k].offset = key.offset;
                    cout << " the student inserted at position = [ " << num << " ]" << "[ "<< k<< " ]" << endl ;
                    cout << "______________________________________________________________\n\n";
                    inserted = true;
                    break;
                }
            }

            num++ ;

            if (num==100)
                num=0;

            if (num==idx) // if it returns back to the beginning position , this means the array is full
            {
                break ;
            }

        }

    }

    void search_by_Name ( char * wanted )
    {

        int position = Hash_1(wanted , 101);

        int  off = search_in_hashed_index ( position , wanted );

        if (off == -1)
        {
            cout << "This student is Not found \n";
            cout << "-----------------------------------\n";
        }
        else
        {
            dataFile.seekg( off , ios::beg);
            student s ;
            dataFile.read( (char*) & s , sizeof (student));
            cout << s ;
            search_1 ++;

        }

    }

    int search_in_hashed_index ( int p , char w [51] )
    {

        bool found = false;
        int indx = p ;

        while ( indx < 101 && found == false)
        {
            for (int k=0 ; k<3 ; k++)
            {
                search_steps_method1++;

                if ( strcmp( hashed_index[indx][k].name , w ) == 0 )
                {
                    found = true;  // will break from the outer while loop
                    return  hashed_index[indx][k].offset ;
                    break;
                }

                if ( strcmp( hashed_index[indx][k].name , "///" ) == 0) // the first empty slot will indicate that the item is not exists
                {
                    return -1 ;
                    break;
                }

            }

            indx++ ;

            if (indx==100)
                indx=0;

            if (indx==p) // array if full searched and it's not found
            {
                return -1 ;
                break ;
            }

        }

    }

    void Delete ( char * wanted)
    {
        int position = Hash_1(wanted , 101);
        int indx = position;
        bool deleted = false ; // to indicate the item to be deleted or the items already deleted

        while ( indx < 101 && deleted == false )
        {
            for (int k=0 ; k<3 ; k++)
            {
                if ( strcmp( hashed_index[indx][k].name , wanted ) == 0 )
                {

                    strcpy(hashed_index[indx][k].name , "***" ); // mark as deleted
                    cout << wanted << " at position = [ " << indx << " ]" << "[ "<< k<< " ] " << "is Deleted !\n";
                    cout << "\n---------------------------------\n\n";
                    deleted = true ;
                    deleted_1++;
                    break;
                }
                else if ( strcmp(hashed_index[indx][k].name , "///" ) == 0 )
                {
                    cout << wanted << " is not found ! \n";
                    cout << "\n---------------------------------\n\n";
                    deleted = true;
                    break;
                }

            }

            indx++ ;

            if (indx==100)
                indx=0;

            if (indx==position) // if it returns back to the original index
            {
                cout << wanted << " is not found ! \n";
                cout << "\n---------------------------------\n\n";
                break;
            }

        }

    }

    void save_hash_array ( )
    {
        fstream f ;
        f.open("Hash.txt" , ios::out|ios::trunc |ios::binary);
        f.seekp(0,ios::beg);

        cout << "******** Saving the Hash Array ******** \n";
        if (f.good())
        {
            for (int i=0 ; i<101 ; i++)
            {
                for (int j=0 ; j<3 ; j++)
                {
                    f.write( (char*) &hashed_index[i][j], sizeof (hashed_index[i][j]));

                    if (strcmp( hashed_index[i][j].name , "///" ) !=0) // just to count the actual data entered the hash file
                    {
                        Entered_hash_file_1 ++;
                    }

                }
            }
        }

        f.flush();
        f.close();
        f.clear();

    }

    void Load_hash_array ()
    {
        fstream hash_file ;
        hash_file.open("Hash.txt" , ios :: in |ios::binary);
        hash_file.seekg(0,ios::end);
        int hash_file_length = hash_file.tellg(); //to know if the hash file is not empty
        hash_file.seekg(0,ios::beg);

        cout << "******** Loading the Hash Array ******** \n";

        if (hash_file_length !=0)
        {
            while ( true )
            {
                for (int i=0 ; i<101 ; i++)
                {
                    for (int j=0 ; j<3 ; j++)
                    {
                        hash_file.read( (char*) & hashed_index[i][j], sizeof (hashed_index[i][j]));

                        if (strcmp( hashed_index[i][j].name , "///" ) !=0) // just to count the actual data entered the hash array
                        {
                            Entered_hash_array_1++;
                        }
                    }
                }

                if (hash_file.eof()) break ;


            }

        }


        hash_file.close();
        hash_file.clear();

    }

    void Load_Data( int operation )
    {
        fstream input_file ("Data to be inserted.txt", ios::in);
        input_file.seekg(0,ios::beg);
        bool do_it = false;

        dataFile.seekg( 0 , ios :: end);
        int dataFile_length = dataFile.tellg();

        if ( (operation==1 && dataFile_length==0) || operation==2 )  // if the data file is empty , read data to insert
            // or if the loading was for search
            do_it = true ;

        if ((operation==1 && dataFile_length!=0)) // if the datafile contains records this means that they have hashed file
        {
            Load_hash_array ();
        }


        if (do_it == true )
        {
            if ( input_file.good() )
            {
                while ( !input_file.eof() )
                {
                    student s ;
                    input_file.getline (s.name , 51);
                    if (input_file.eof()) break;
                    input_file.getline (s.gpa , 4);
                    input_file.getline (s.address , 51);
                    input_file.getline (s.mobile , 12);
                    input_file.getline (s.department , 3);

                    if (operation == 1)  // load to insert data in the file
                    {
                        add_student (s);
                    }
                    else if (operation == 2) // load to search for inserted data
                        search_by_Name(s.name );
                }
            }

            input_file.close();
        }

    }

    void Load_Data_to_delete()
    {
        fstream in ;
        in.open("Data to Delete.txt" , ios::in);
        in.seekg (0 , ios :: beg);
        if ( in.good() )
        {
            while ( !in.eof() )
            {
                student s ;
                in.getline (s.name , 51);
                if (in.eof()) break;
                in.getline (s.gpa , 4);
                in.getline (s.address , 51);
                in.getline (s.mobile , 12);
                in.getline (s.department , 3);

                Delete ( s.name );
            }
        }

        in.close();


        cout << "***************** Inserting New Records From 28 to 42 *******************\n";

        insert_new_data ();

    }

    void insert_new_data ()
    {

        fstream f;
        f.open("New Data.txt", ios::in);
        f.seekg(0,ios::beg);
        if (f.good())
        {
            student s ;

            while( ! f.eof()  )
            {

                f.getline (s.name , 51);
                if (f.eof()) break ;
                f.getline (s.gpa , 4);
                f.getline (s.address , 51);
                f.getline (s.mobile , 12);
                f.getline (s.department , 3);

                add_student(s);

            }

        }

        f.close();
    }

    void Load_Data_ALL ()
    {
        fstream input_file ("ALL Data.txt", ios::in);
        input_file.seekg(0,ios::beg);

        if ( input_file.good() )
        {
            while ( !input_file.eof() )
            {
                student s ;
                input_file.getline (s.name , 51);
                if (input_file.eof()) break;
                input_file.getline (s.gpa , 4);
                input_file.getline (s.address , 51);
                input_file.getline (s.mobile , 12);
                input_file.getline (s.department , 3);

                search_by_Name(s.name );
            }
        }

        input_file.close();

    }

    void print_Busy_cells ()
    {
        for (int i=0 ; i<101 ; i++)
        {
            for (int j=0 ; j<3 ; j++)
            {
                if (strcmp(hashed_index[i][j].name , "///") !=0  && strcmp(hashed_index[i][j].name , "***") !=0 )
                {
                    cout << "Name = " << hashed_index[i][j].name <<endl ;
                    cout << "offset = " << hashed_index[i][j].offset <<endl ;
                    cout << "Index = [ "<< i << " ]" << "[ " << j<< " ]\n";
                    cout << "---------------------------------------------\n";
                    actual_size_1 ++ ;
                }
            }
        }

    }

    void print_data_file ()
    {
        fstream f ;
        f.open("test.txt" , ios :: in );
        f.seekg(0,ios::beg);
        cout << "\n\n**************** Data of File **************\n\n";
        while (! f.eof())
        {
            student t ;
            f.read((char*) &t , sizeof(student));
            if (f.eof()) break ;
            cout << t ;

        }
        cout << "\n\n**************** End of File **************\n\n";

        f.flush();
        f.clear();
        f.close();
    }

    ~method1 ()
    {
        save_hash_array();
        dataFile.flush();
        dataFile.close();
        dataFile.clear();
    }

};


class method2
{
    PIndex double_hashed_index [101][2];
    fstream dataFile;

public:

    method2 (char* dFile)
    {
        dataFile.open(dFile,ios::in|ios::out);

        for (int i=0 ; i<101 ; i++)
        {
            for (int j=0 ; j<2 ; j++)
            {

                strcpy( double_hashed_index [i][j].name , "///" );
                double_hashed_index [i][j].offset = -1 ;
            }
        }
    }

    void add_student (student & s )
    {

        dataFile.seekp(0,ios::end);
        int of = dataFile.tellp();
        dataFile.write( (char*) & s , sizeof (student));

        PIndex key ;
        strcpy(key.name , s.name);
        key.offset = of;

        cout << "\n********** Adding students ***********\n\n";
        cout << "key = " << key.name << "  , offset = " << key.offset <<endl ;


        int index = Hash_1 ( key.name , 101);

        cout << "index of hash function = " << index <<endl;

        insert_Hashed_index (index , key);

    }

    void insert_Hashed_index (int idx , PIndex key  )
    {
        bool inserted = false , full = false ;
        int num = idx ;
        int indx2 = Hash_2 (key.name , 23);
        int step = 1 ;
        int base = num ;  // this is the constant value for displacement ( Al 2eza7a)

        while (inserted == false )
        {
            for (int k=0 ; k<2 ; k++)
            {
                if ( strcmp( double_hashed_index[num][k].name , "///" ) == 0 || strcmp(double_hashed_index[num][k].name ,"***" )==0 )
                {
                    strcpy(this->double_hashed_index[num][k].name , key.name );
                    this->double_hashed_index[num][k].offset = key.offset;
                    cout << " the student inserted at position = [ " << num << " ]" << "[ "<< k<< " ]" << endl ;
                    cout << "______________________________________________________________\n\n";
                    inserted = true;
                    break;
                }

            }

            num = ( base + (step*indx2)) % 101 ;
            step++;


            if (num==idx)
                break;

        }

    }


    void search_by_Name ( char * wanted )
    {

        int position = Hash_1(wanted , 101);

        int  off = search_in_hashed_index ( position , wanted );

        if (off == -1)
        {
            cout << "This student is Not found \n";
            cout << "-----------------------------------\n";
        }
        else
        {
            dataFile.seekg( off , ios::beg);
            student s ;
            dataFile.read( (char*) & s , sizeof (student));
            cout << s ;
            search_2 ++;

        }

    }

    int search_in_hashed_index ( int p , char w [51] )
    {

        bool found = false;
        int indx = p ;
        int step = 1;

        while (found == false)
        {
            for (int k=0 ; k<2 ; k++)
            {

                search_steps_method2++;

                if ( strcmp( double_hashed_index[indx][k].name , w ) == 0 )
                {
                    found = true;
                    return  double_hashed_index[indx][k].offset ;
                    break;
                }

                if (strcmp( double_hashed_index[indx][k].name , "///" ) == 0)
                {
                    return -1 ;
                    break ;
                }


            }

            indx = ( p + (step*Hash_2(w,23))) % 101 ;
            step++;

        }

    }

    void Delete ( char * wanted)
    {
        int position = Hash_1(wanted , 101);
        int indx = position;
        bool deleted = false ;

        int step = 1;

        while (deleted == false )
        {
            for (int k=0 ; k<2 ; k++)
            {

                if ( strcmp( double_hashed_index[indx][k].name , wanted ) == 0 )
                {
                    strcpy( double_hashed_index[indx][k].name , "***" );
                    cout << wanted << " at position = [ " << indx << " ]" << "[ "<< k<< " ] " << "is Deleted !\n";
                    cout << "\n---------------------------------\n\n";
                    deleted = true ;
                    deleted_2++;
                    break;
                }

                else if ( strcmp( double_hashed_index[indx][k].name , "///" ) == 0 )
                {
                    cout << wanted << " is not found ! \n";
                    cout << "\n---------------------------------\n\n";
                    deleted = true ;
                    break;
                }

            }

            indx = ( position + (step*Hash_2(wanted,23))) % 101 ;
            step++;


        }


    }

    void save_hash_array ( )
    {
        fstream f ;
        f.open("Hash.txt" , ios::out|ios::trunc |ios::binary);
        f.seekp(0,ios::beg);

        cout << "******** Saving the Hash Array ******** \n";
        if (f.good())
        {
            for (int i=0 ; i<101 ; i++)
            {
                for (int j=0 ; j<2 ; j++)
                {
                    f.write( (char*) &double_hashed_index[i][j], sizeof (double_hashed_index[i][j]));

                    if (strcmp( double_hashed_index[i][j].name , "///" ) !=0)
                    {
                        Entered_hash_file_2 ++;
                    }

                }
            }

        }

        f.flush();
        f.close();
        f.clear();

    }

    void Load_hash_array ()
    {

        fstream hash_file ;
        hash_file.open("Hash.txt" , ios :: in |ios::binary);
        hash_file.seekg(0,ios::end);
        int hash_file_length = hash_file.tellg();
        hash_file.seekg(0,ios::beg);

        cout << "******** Loading the Hash Array ******** \n";

        if (hash_file_length !=0)
        {
            while ( !hash_file.eof() )
            {

                for (int i=0 ; i<101 ; i++)
                {
                    for (int j=0 ; j<2 ; j++)
                    {
                        hash_file.read( (char*) & double_hashed_index[i][j], sizeof (double_hashed_index[i][j]));

                        if (strcmp( double_hashed_index[i][j].name , "///" ) !=0)
                        {
                            Entered_hash_array_2++;
                        }
                    }
                }

                if (hash_file.eof()) break ;

            }


        }


        hash_file.close();
        hash_file.clear();

    }

    void Load_Data( int operation )
    {
        fstream input_file ("Data to be inserted.txt", ios::in);
        input_file.seekg(0,ios::beg);
        bool do_it = false;

        dataFile.seekg( 0 , ios :: end);
        int dataFile_length = dataFile.tellg();

        if ( (operation==1 && dataFile_length==0) || operation==2 )  // if the data file is empty , read data to insert
            // or if the loading was for search
            do_it = true ;

        if ((operation==1 && dataFile_length!=0)) // if the datafile contains records this means that they have hashed file
        {
            Load_hash_array ();
        }


        if (do_it == true )
        {
            if ( input_file.good() )
            {
                while ( !input_file.eof() )
                {
                    student s ;
                    input_file.getline (s.name , 51);
                    if (input_file.eof()) break;
                    input_file.getline (s.gpa , 4);
                    input_file.getline (s.address , 51);
                    input_file.getline (s.mobile , 12);
                    input_file.getline (s.department , 3);

                    if (operation == 1)  // load to insert data in the file
                    {
                        add_student (s);
                    }
                    else if (operation == 2) // load to search for inserted data
                        search_by_Name(s.name );
                }
            }

            input_file.close();
        }

    }

    void Load_Data_to_delete()
    {
        fstream in ;
        in.open("Data to Delete.txt" , ios::in);
        in.seekg (0 , ios :: beg);
        if ( in.good() )
        {
            while ( !in.eof() )
            {
                student s ;
                in.getline (s.name , 51);
                if (in.eof()) break;
                in.getline (s.gpa , 4);
                in.getline (s.address , 51);
                in.getline (s.mobile , 12);
                in.getline (s.department , 3);

                Delete ( s.name );
            }
        }

        in.close();

        insert_new_data ();

    }

    void insert_new_data ()
    {

        fstream f;
        f.open("New Data.txt", ios::in);
        f.seekg(0,ios::beg);
        if (f.good())
        {
            student s ;

            while( ! f.eof()  )
            {

                f.getline (s.name , 51);
                if (f.eof()) break ;
                f.getline (s.gpa , 4);
                f.getline (s.address , 51);
                f.getline (s.mobile , 12);
                f.getline (s.department , 3);

                add_student(s);

            }

        }

        f.close();
    }


    void Load_Data_ALL ()
    {
        fstream input_file ("ALL Data.txt", ios::in);
        input_file.seekg(0,ios::beg);

        if ( input_file.good() )
        {
            while ( !input_file.eof() )
            {
                student s ;
                input_file.getline (s.name , 51);
                if (input_file.eof()) break;
                input_file.getline (s.gpa , 4);
                input_file.getline (s.address , 51);
                input_file.getline (s.mobile , 12);
                input_file.getline (s.department , 3);

                search_by_Name(s.name );
            }
        }

        input_file.close();

    }

    void print_Busy_cells ()
    {
        for (int i=0 ; i<101 ; i++)
        {
            for (int j=0 ; j<2 ; j++)
            {
                if (strcmp(double_hashed_index[i][j].name , "///") !=0 &&  strcmp(double_hashed_index[i][j].name , "***") !=0 )
                {

                    cout << "Name = " << double_hashed_index[i][j].name <<endl ;
                    cout << "offset = " << double_hashed_index[i][j].offset <<endl ;
                    cout << "Index = [ "<< i << " ]" << "[ " << j<< " ]\n";
                    cout << "---------------------------------------------\n";
                    actual_size_2++ ;
                }
            }
        }

    }

    void print_data_file ()
    {
        fstream f ;
        f.open("test.txt" , ios :: in );
        f.seekg(0,ios::beg);
        cout << "\n\n**************** Data of File **************\n\n";
        while (! f.eof())
        {
            student t ;
            f.read((char*) &t , sizeof(student));
            if (f.eof()) break ;
            cout << t ;

        }
        cout << "\n\n**************** End of File **************\n\n";

        f.flush();
        f.clear();
        f.close();
    }
    ~method2()
    {
        save_hash_array();
        dataFile.flush();
        dataFile.close();
        dataFile.clear();
    }

};

class method3
{
    int scatter_table [101];
    fstream dataFile;

public:

    method3 (char* dFile)
    {
        dataFile.open(dFile,ios::in|ios::out |ios::binary);
        for (int i=0 ; i<100 ; i++)
        {
            scatter_table[i]=-1;
        }

    }

    void add_student (student3 & s)
    {
        dataFile.seekg(0 , ios::end);
        int off = dataFile.tellg();
        s.offset = off ;
        s.next = -1 ;

        int index = Hash_1(s.name , 101);

        dataFile.seekp (0 , ios::end);
        dataFile.write( (char*) & s , 132 );

        cout << "\n********** Adding students ***********\n\n";
        cout << "key = " << s.name << "  , offset = " << s.offset << endl ;
        cout << "index of hash function = " << index <<endl;
        cout << "------------------------------------\n\n";

        if (scatter_table[index] == -1)     // this is the header of the chain
        {
            scatter_table[index] = off ;

            cout << "scatter table at " << index << " = " << off << endl ;
            cout<< "---------------------------------------\n";
        }
        else
        {

            int temp = scatter_table[index];

            while (true)
            {
                student3 current ;

                dataFile.seekg( temp , ios::beg);
                dataFile.read ( (char*) & current , sizeof (student3));

                cout << "----------------------------------\n";
                cout << "current = " << current.name << endl ;
                cout << "Next = "<< current.next << endl ;
                cout << "----------------------------------\n";

                if (current.next == -1)   // insert at the end of file ( -1 ) is the last element of the chain then connect them again
                {
                    current.next = s.offset;
                    dataFile.seekp ( -4 , ios::cur );
                    dataFile.write ( (char*) & current.next , 4 );

                    cout << "*********** The Record is inserted *********** \n";
                    break;
                }

                else
                {
                    temp = current.next ;  // read until the end of the chain

                }

            }

        }

    }

    void search_by_Name ( char * wanted )
    {

        int offset = search_in_chain( wanted );

        if (offset == -1)
        {
            cout << "This student is Not Found !\n";
            cout << "-----------------------------------\n";

        }
        else
        {
            student3 s ;
            dataFile.seekg ( offset , ios ::beg);
            dataFile.read ( (char*) &s , sizeof (s));
            cout << s ;
            search_3++;

        }

    }

    int search_in_chain ( char * wanted )
    {
        int index = Hash_1 ( wanted , 101);

        if (scatter_table[index] == -1 )  // there is no element in this chain
            return -1 ;

        else
        {
            int temp = scatter_table[index];   // read the head of the chain

            while (true)
            {
                student3 s ;

                dataFile.seekg ( temp , ios::beg);
                dataFile.read ( (char*) & s , sizeof (s));

                search_steps_method3++;

                if ( strcmp (s.name , wanted ) == 0 )
                {
                    search_3++ ; // number of student that have been searched for
                    return temp ; // return offset
                    break ;
                }

                else if ( s.next == -1) // this is the last element in the chain and the student is not found
                {
                    return -1 ;
                    break ;
                }
                else
                {
                    temp = s.next ;  // keep reading the offset of the next element in the chain
                }

            }

        }

    }

    void Delete ( char * wanted )
    {
        int offset = search_in_chain (wanted);

        if (offset == -1)
        {
            cout << "This student is Not Found !\n";
            cout << "\n---------------------------------\n";

        }
        else
        {
            char mark = '*';
            student3 deleted ;

            dataFile.seekp ( offset , ios ::beg);
            dataFile.write ( & mark , 1 );   // overwrite (*) to mark the deleted Record
            deleted_3++;

            cout << wanted << " is Deleted !\n";
            cout << "\n---------------------------------\n\n";

            dataFile.seekg ( -1  , ios:: cur); // to go back word because of mark (*)
            dataFile.read ( (char*) &deleted , sizeof (deleted)); // to read next of deleted record

            int index = Hash_1 (wanted , 101);

            if (scatter_table [index] == offset)  // the deleted item is the first one in the chain so connect it to the next of deleted item
            {
                scatter_table [index] = deleted.next ;
            }

            else
            {
                int Prev_of_deleted = scatter_table [index] ;  // read the previous element

                while (true)

                {

                    student3 next_of_deleted ;

                    dataFile.seekg ( Prev_of_deleted , ios::beg);
                    dataFile.read ( (char*) & next_of_deleted , sizeof (next_of_deleted));

                    if (next_of_deleted.next == offset)  // if the deleted item is in the middle connect the previous to next of deleted item
                    {
                        dataFile.seekp( -4 , ios::cur );
                        dataFile.write ( (char*) & deleted.next , 4 );
                        break ;

                    }

                    else
                    {
                        Prev_of_deleted = next_of_deleted.next; //keep reading until reach the wanted item offset

                    }
                }

            }
        }
    }

    void save_hash_array ( )
    {

        fstream f ;
        f.open("Hash.txt" , ios::out|ios::trunc|ios::binary);
        f.seekp(0,ios::beg);

        if (f.good())
        {

            for (int i=0 ; i<101 ; i++)
            {

                f.write( (char*) &scatter_table[i], sizeof (scatter_table[i]));

                if (scatter_table[i]!=-1)
                {
                    Entered_hash_file_3++;
                }

            }

        }


        f.close();

    }

    void Load_hash_array ()
    {

        fstream hash_file ;
        hash_file.open("Hash.txt" , ios :: in |ios::binary );
        hash_file.seekg(0,ios::end);
        int hash_file_length = hash_file.tellg();
        hash_file.seekg(0,ios::beg);

        cout << "******** Loading the Hash Array ******** \n";

        if (hash_file_length !=0)
        {
            while ( true )
            {
                for (int i=0 ; i<101 ; i++)
                {
                    hash_file.read( (char*) & scatter_table[i], sizeof (scatter_table[i]));

                    if (scatter_table[i]!=-1)
                    {
                        Entered_hash_array_3++;
                    }
                }
                if (hash_file.eof()) break ;
            }
        }


        hash_file.close();

    }

    void Load_Data(int operation )
    {
        fstream input_file ("Data to be inserted.txt", ios::in);
        input_file.seekg(0,ios::beg);
        bool do_it = false;
        dataFile.seekg( 0 , ios :: end);
        int dataFile_length = dataFile.tellg();

        if ( (operation==1 && dataFile_length==0) || operation==2 )
            do_it = true ;

        if ((operation==1 && dataFile_length!=0))
        {
            Load_hash_array ();
        }

        if (do_it == true )
        {
            if ( input_file.good() )
            {
                while ( !input_file.eof() )
                {
                    student3 s ;
                    input_file.getline (s.name , 51);
                    if (input_file.eof()) break;
                    input_file.getline (s.gpa , 4);
                    input_file.getline (s.address , 51);
                    input_file.getline (s.mobile , 12);
                    input_file.getline (s.department , 3);

                    if (operation == 1)  // load to insert data in the file
                    {
                        add_student (s);
                    }
                    else if (operation == 2) // load to search for inserted data
                        search_by_Name ( s.name );
                }
            }

            input_file.close();
        }
    }

    void Load_Data_to_delete()
    {
        fstream in ;
        in.open("Data to Delete.txt" , ios::in);
        in.seekg (0 , ios :: beg);
        if ( in.good() )
        {
            while ( !in.eof() )
            {
                student s ;
                in.getline (s.name , 51);
                if (in.eof()) break;
                in.getline (s.gpa , 4);
                in.getline (s.address , 51);
                in.getline (s.mobile , 12);
                in.getline (s.department , 3);

                Delete ( s.name );
            }
        }

        in.close();

        insert_new_data ();

    }

    void insert_new_data ()
    {
        fstream f;
        f.open("New Data.txt", ios::in);
        f.seekg(0,ios::beg);
        if (f.good())
        {
            student3 s ;

            while( ! f.eof()  )
            {

                f.getline (s.name , 51);
                if (f.eof()) break ;
                f.getline (s.gpa , 4);
                f.getline (s.address , 51);
                f.getline (s.mobile , 12);
                f.getline (s.department , 3);

                add_student(s);

            }

        }

        f.close();
    }

    void Load_Data_ALL ()
    {
        fstream input_file ("ALL Data.txt", ios::in);
        input_file.seekg(0,ios::beg);

        if ( input_file.good() )
        {
            while ( !input_file.eof() )
            {
                student s ;
                input_file.getline (s.name , 51);
                if (input_file.eof()) break;
                input_file.getline (s.gpa , 4);
                input_file.getline (s.address , 51);
                input_file.getline (s.mobile , 12);
                input_file.getline (s.department , 3);

                search_by_Name(s.name );
            }
        }

        input_file.close();

    }

    void print_Busy_cells ()
    {
        for (int i=0 ; i<101 ; i++)
        {

            if ( scatter_table[i] != -1 )
            {

                cout << "offset = " << scatter_table[i]<<endl ;
                cout << "---------------------------------------------\n";
                actual_size_3++ ;
            }

        }

    }

    void print_data_file ()
    {
        fstream f ;
        f.open("test.txt" , ios :: in );
        f.seekg(0,ios::beg);
        cout << "\n\n**************** Data of File **************\n\n";
        while (! f.eof())
        {
            student3 t ;
            f.read((char*) &t , sizeof(student3));
            cout << t ;
            if (f.eof()) break ;

        }
        cout << "\n\n**************** End of File **************\n\n";

        f.flush();
        f.clear();
        f.close();
    }


    ~method3()
    {
        save_hash_array();
        dataFile.flush();
        dataFile.close();
        dataFile.clear();
    }

};

int main()
{

/*
    method1 class1 ("test.txt");

    cout << "**************** Inserting the Records To the Data File *******************\n\n";
    class1.Load_Data(1); // insert Records

    cout << "**************** Searching for the Records To the Data File *****************\n\n";
    class1.Load_Data(2); // search for inserted Records

    cout <<"Average search BEFORE Deletion = " << (float) search_steps_method1 / 27 << endl ;
    cout << "\n----------------------------------------------------------\n";

//    cout << "****************** Deleting Records From 13 to 27 ********************\n";
//    class1.Load_Data_to_delete ();
//
//    search_steps_method1 =0;
//    cout << "**************** Searching for the Records After Delete *****************\n";
//    class1.Load_Data_ALL();
//
//    cout <<"Average search AFTER Deletion = " << (float) search_steps_method1 / 42 << endl ;
//    cout << "\n***********************************************************************\n\n";



*/




    method2 class2 ("test.txt");

    cout << "**************** Inserting the Records To the Data File *******************\n\n";
    class2.Load_Data(1); // insert Records

    cout << "**************** Searching for the Records To the Data File *****************\n\n";
    class2.Load_Data(2); // search for inserted Records

    cout <<"Average search BEFORE Deletion = " << (float) search_steps_method2 / 27 << endl ;
    cout << "\n----------------------------------------------------------\n";

//    cout << "****************** Deleting Records From 13 to 27 ********************\n";
//    class2.Load_Data_to_delete ();
//
//    search_steps_method2 =0;
//    cout << "**************** Searching for the Records After Delete *****************\n";
//    class2.Load_Data_ALL();
//
//    cout <<"Average search AFTER Deletion = " << (float) search_steps_method2 / 42 << endl ;
//    cout << "\n***********************************************************************\n\n";




/*

    method3 class3 ("test.txt");

    cout << "**************** Inserting the Records To the Data File *******************\n\n";
    class3.Load_Data(1); // insert Records

    cout << "**************** Searching for the Records To the Data File *****************\n\n";
    class3.Load_Data(2); // search for inserted Records

    cout <<"Average search BEFORE Deletion = " << (float) search_steps_method3 / 27 << endl ;
    cout << "\n----------------------------------------------------------\n";

//    cout << "****************** Deleting Records From 13 to 27 ********************\n";
//    class3.Load_Data_to_delete ();
//
//    search_steps_method3 =0;
//    cout << "**************** Searching for the Records After Delete *****************\n";
//    class3.Load_Data_ALL();
//
//    cout <<"Average search AFTER Deletion = " << (float) search_steps_method3 / 42 << endl ;
//    cout << "\n***********************************************************************\n\n";


*/
    return 0;

}
