#ifndef INITDB_H
#define INITDB_H

#include <QtSql>
#pragma execution_character_set("utf-8")
//数据库类

//添加值到值列表中，执行sql查询。
void addBook(QSqlQuery &q ,const QString &title, int year, const QVariant &authorId,const QVariant &genreId, int rating){
    //QSqlQuery执行sql语句。
    q.addBindValue(title); //使用位置值绑定，将参数值添加到值列表中
    q.addBindValue(year);
    q.addBindValue(authorId);
    q.addBindValue(genreId);
    q.addBindValue(rating);
    q.exec(); //执行先前准备好的sql
}

QVariant addGenre(QSqlQuery &q, const QString &name){
    q.addBindValue(name);
    q.exec();
    return q.lastInsertId(); //如果数据库支持，则返回最近插入行的对象ID。
}

//添加名字和日期
QVariant addAuthor(QSqlQuery &q,const QString &name,QDate birthdate){
    q.addBindValue(name);
    q.addBindValue(birthdate); //添加日期值
    q.exec();
    return  q.lastInsertId(); //返回最近插入行的对象ID
}

//QLatin1String类为US-ASCII/Latin-1编码的字符串文本提供了一个精简的包装器
//创建表。R代表查询
const auto BOOKS_SQL = QLatin1String(R"(create table books(id integer primary key, title varchar, author integer,genre integer, year integer, rating integer))");

const auto AUTHORS_SQL =  QLatin1String(R"(
    create table authors(id integer primary key, name varchar, birthdate date)
    )");

const auto GENRES_SQL = QLatin1String(R"(
    create table genres(id integer primary key, name varchar)
    )");

//插入
const auto INSERT_AUTHOR_SQL = QLatin1String(R"(
    insert into authors(name, birthdate) values(?, ?)
    )");

const auto INSERT_BOOK_SQL = QLatin1String(R"(
    insert into books(title, year, author, genre, rating)
                      values(?, ?, ?, ?, ?)
    )");

const auto INSERT_GENRE_SQL = QLatin1String(R"(
    insert into genres(name) values(?)
    )");



QSqlError initDb(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:"); //数据库名
//    db.setHostName("localhost"); //主机名
//    //db.setPort(3306);
//    db.setUserName("root"); //用户名
//    db.setPassword("111");
    if(!db.open()){
        return db.lastError();//返回关于数据库上发生的最后一个错误的信息。
    }

    //返回由参数类型指定的数据库表、系统表和视图的列表
    QStringList tables = db.tables();
    //contains列表中包含指定的字符串返回true.CaseInsensitive搜索时不区分大小写
    if(tables.contains("books",Qt::CaseInsensitive) && tables.contains("authors",Qt::CaseInsensitive)) //没用这两个表时
        return QSqlError();


    QSqlQuery q; //下面创建3个表
    if(!q.exec(BOOKS_SQL))
        return q.lastError(); //返回发生的最后一个错误信息
    if(!q.exec(AUTHORS_SQL))
        return q.lastError();
    if(!q.exec(GENRES_SQL))
        return q.lastError();


    if(!q.prepare(INSERT_AUTHOR_SQL)) //准备执行的sql
        return q.lastError();
    //向表插入数据.QLatin1String用中文会乱码，优点是也许会更快。先用QString
    QVariant asimovId = addAuthor(q,QLatin1String("Ivor Horton"),QDate(2009,10,1));
    QVariant greeneId = addAuthor(q,QString("刘晓霞"),QDate(2003,10,1));
    QVariant pratchettId = addAuthor(q,QString("霍亚飞"),QDate(2001,10,1));


    if(!q.prepare(INSERT_GENRE_SQL))
        return q.lastError();
    QVariant sfiction = addGenre(q, QString("C语言"));
    QVariant fiction = addGenre(q, QLatin1String("Mysql"));
    QVariant fantasy = addGenre(q, QLatin1String("Qt"));


    if(!q.prepare(INSERT_BOOK_SQL)) //插入表数据
        return q.lastError();

    addBook(q,QString("C语言入门经典"),1951,asimovId,sfiction,3);
    addBook(q,QString("c数据结构"),1952,asimovId,sfiction,4);
    addBook(q,QString("C语言入门经典"),1943,asimovId,sfiction,2);
    addBook(q,QLatin1String("Second Foundation"),1943,asimovId,sfiction,3);
    addBook(q,QString("c陷阱"),1933,asimovId,sfiction,2);
    addBook(q,QString("C语言入门经典"),1923,asimovId,sfiction,4);
    addBook(q, QString("Mysql必知必会"), 1940, greeneId, fiction, 4);
    addBook(q, QLatin1String("The Third Man"), 1950, greeneId, fiction, 5);
    addBook(q, QLatin1String("Our Man in Havana"), 1958, greeneId, fiction, 4);
    addBook(q, QString("Qt快速入门"), 1989, pratchettId, fantasy, 3);
    addBook(q, QLatin1String("Night Watch"), 2002, pratchettId, fantasy, 3);
    addBook(q, QLatin1String("Going Postal"), 2004, pratchettId, fantasy, 3);

    return QSqlError();
}

#endif // INITDB_H
