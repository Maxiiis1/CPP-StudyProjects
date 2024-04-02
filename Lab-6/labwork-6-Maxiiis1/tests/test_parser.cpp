#include <lib/parser.h>

#include <gtest/gtest.h>
#include <sstream>

TEST(MyBdTest, Create) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE People (Age INT, Name STRING, 18 BOOL, Job VARCHAR(10) PRIMARY KEY(man_id))");

    Table tt("test");
    tt = new_base.GetTable("People");

    ASSERT_EQ("People",tt.GetName());

    ASSERT_EQ("man_id",tt.GetColumnNames()[0]);
    ASSERT_EQ("Age",tt.GetColumnNames()[1]);
    ASSERT_EQ("Name",tt.GetColumnNames()[2]);
    ASSERT_EQ("18",tt.GetColumnNames()[3]);
    ASSERT_EQ("Job",tt.GetColumnNames()[4]);
}


TEST(MyBdTest, Insert) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE People (Age INT, Name STRING, 18 BOOL, Job VARCHAR(10) PRIMARY KEY(man_id))");
    new_base.Query("INSERT INTO People (man_id, Age, Name, 18, Job) VALUES (1, 18, Mike, true, Carwashing)");

    Table tt("test");
    tt = new_base.GetTable("People");

    int i = 0;

    ASSERT_EQ("1",tt.GetValue("man_id", i));
    ASSERT_EQ("18",tt.GetValue("Age", i));
    ASSERT_EQ("Mike",tt.GetValue("Name", i));
    ASSERT_EQ("true",tt.GetValue("18", i));
    ASSERT_EQ("Carwashing",tt.GetValue("Job", i));
}

TEST(MyBdTest, NotFullInsert) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE People (Age INT, Name STRING, 18 BOOL, Job VARCHAR(10) PRIMARY KEY(man_id))");
    new_base.Query("INSERT INTO People (man_id, Age, Name, 18, Job) VALUES (1, 18, Mike, true)");
    new_base.Query("INSERT INTO People (man_id, Age, Name, 18, Job) VALUES (2, 17)");

    Table tt("test");
    tt = new_base.GetTable("People");

    int i = 0;

    ASSERT_EQ("1",tt.GetValue("man_id", i));
    ASSERT_EQ("18",tt.GetValue("Age", i));
    ASSERT_EQ("Mike",tt.GetValue("Name", i));
    ASSERT_EQ("true",tt.GetValue("18", i));
    ASSERT_EQ("NULL",tt.GetValue("Job", i));

    i++;
    ASSERT_EQ("2",tt.GetValue("man_id", i));
    ASSERT_EQ("17",tt.GetValue("Age", i));
    ASSERT_EQ("NULL",tt.GetValue("Name", i));
    ASSERT_EQ("false",tt.GetValue("18", i));
    ASSERT_EQ("NULL",tt.GetValue("Job", i));
}

TEST(MyBdTest, Values) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE People (Age INT, Name STRING, 18 BOOL)");
    new_base.Query("INSERT INTO People  VALUES (17, Andrew, false)");


    Table tt("test");
    tt = new_base.GetTable("People");
    int i = 0;

    ASSERT_EQ("17",tt.GetValue("Age", i));
    ASSERT_EQ("Andrew",tt.GetValue("Name", i));
    ASSERT_EQ("false",tt.GetValue("18", i));

}

TEST(MyBdTest, Drop) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE People (Age INT, Name STRING, 18 BOOL)");
    new_base.Query("INSERT INTO People  VALUES (17, Andrew, false)");

    new_base.Query("DROP TABLE People");

    Table tt("test");
    tt = new_base.GetTable("People");

    ASSERT_EQ("",tt.GetName());
}

TEST(MyBdTest, Delete) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE People (Age INT, Name STRING, 18 BOOL)");
    new_base.Query("INSERT INTO People  VALUES (17, Andrew, false)");

    size_t size = 3;

    Table tt("test");
    tt = new_base.GetTable("People");

    ASSERT_EQ(size,tt.GetColumnCount());

    new_base.Query("DELETE FROM TABLE Name");
    ASSERT_EQ(size,tt.GetColumnCount());
}

TEST(MyBdTest, Select) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE People (Age INT, Name STRING, 18 BOOL)");
    new_base.Query("INSERT INTO People  VALUES (17, Andrew, false)");
    new_base.Query("INSERT INTO People  VALUES (15, Mike, false)");
    new_base.Query("INSERT INTO People  VALUES (12, Alice, false)");
    new_base.Query("SELECT * FROM People");

    Table tt("test");
    tt = new_base.GetTable("People");

    int i = 0;

    ASSERT_EQ("17",tt.GetValue("Age", i));
    ASSERT_EQ("Andrew",tt.GetValue("Name", i));
    ASSERT_EQ("false",tt.GetValue("18", i));

    i++;
    ASSERT_EQ("15",tt.GetValue("Age", i));
    ASSERT_EQ("Mike",tt.GetValue("Name", i));
    ASSERT_EQ("false",tt.GetValue("18", i));

    i++;
    ASSERT_EQ("12",tt.GetValue("Age", i));
    ASSERT_EQ("Alice",tt.GetValue("Name", i));
    ASSERT_EQ("false",tt.GetValue("18", i));

}

TEST(MyBdTest, InnerJoin) {
    Database new_base("Hi");
    new_base.Query("CREATE TABLE Children (Age INT, Name STRING, 18 BOOL)");
    new_base.Query("INSERT INTO Children  VALUES (17, Andrew, false)");

    new_base.Query("CREATE TABLE Parents (Age INT, Name STRING, Job VARCHAR(10))");
    new_base.Query("INSERT INTO Parents  VALUES (44, Mike, Influencer)");

    new_base.Query("SELECT (Age, Name, 18) FROM Children JOIN INNER Parents ON People");

    Table table1("test1");
    table1 = new_base.GetTable("People");

    ASSERT_EQ("People", table1.GetName());
    ASSERT_EQ("Age" ,table1.GetColumnNames()[0]);
    ASSERT_EQ("Name", table1.GetColumnNames()[1]);


}

TEST(MyBdTest, LeftJoin) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE Children (Age INT, Name STRING, 18 BOOL)");
    new_base.Query("INSERT INTO Children  VALUES (17, Andrew, false)");

    new_base.Query("CREATE TABLE Parents (Age INT, Name STRING, Job VARCHAR(10))");
    new_base.Query("INSERT INTO Parents  VALUES (44, Mike, Influencer)");

    new_base.Query("SELECT (Age, Name, 18) FROM Children JOIN LEFT Parents ON People");

    Table table1("test1");
    table1 = new_base.GetTable("People");

    ASSERT_EQ("People", table1.GetName());

    ASSERT_EQ("Age" ,table1.GetColumnNames()[0]);
    ASSERT_EQ("Name", table1.GetColumnNames()[1]);
    ASSERT_EQ("18", table1.GetColumnNames()[2]);

    int i = 0;

    ASSERT_EQ("17",table1.GetValue("Age", i));
    ASSERT_EQ("Andrew",table1.GetValue("Name", i));
    ASSERT_EQ("false",table1.GetValue("18", i));
}


TEST(MyBdTest, RightJoin) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE Children (Age INT, Name STRING, 18 BOOL)");
    new_base.Query("INSERT INTO Children  VALUES (17, Andrew, false)");

    new_base.Query("CREATE TABLE Parents (Age INT, Name STRING, Job VARCHAR(10))");
    new_base.Query("INSERT INTO Parents  VALUES (44, Mike, Influencer)");

    new_base.Query("SELECT (Age, Name, 18) FROM Children JOIN RIGHT Parents ON People");

    Table table1("test1");
    table1 = new_base.GetTable("People");

    ASSERT_EQ("People", table1.GetName());

    ASSERT_EQ("Age" ,table1.GetColumnNames()[0]);
    ASSERT_EQ("Name", table1.GetColumnNames()[1]);
    ASSERT_EQ("Job", table1.GetColumnNames()[2]);

    int i = 0;

    ASSERT_EQ("44",table1.GetValue("Age", i));
    ASSERT_EQ("Mike",table1.GetValue("Name", i));
    ASSERT_EQ("Influencer",table1.GetValue("Job", i));
}

TEST(MyBdTest, Update) {
    Database new_base("Hi");

    new_base.Query("CREATE TABLE Children (Age INT, Name STRING, Education STRING)");
    new_base.Query("INSERT INTO Children  VALUES (17, Andrew, School)");
    new_base.Query("INSERT INTO Children  VALUES (19, Mike, School)");
    new_base.Query("UPDATE Children SET Education=University WHERE Age>18" );

    Table table1("test1");
    table1 = new_base.GetTable("Children");

    ASSERT_EQ("Children", table1.GetName());
    int i = 0;

    ASSERT_EQ("University",table1.GetValue("Education", i));

}