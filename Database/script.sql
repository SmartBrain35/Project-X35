--table to contain new user i.e who operate the software
CREATE TABLE ADD_USER
(
	UserId varchar(50) PRIMARY KEY NOT NULL,
	UserName varchar(50) NOT NULL,
	Status varchar(12) NOT null,
	Phone varchar(10) NOT null,
	Email varchar(20) NOT null,
	Pass varchar(20) NOT null,
	Photo BYTEA,
	Issued_Date DATE,

    constraint unique_email UNIQUE(Email),
	constraint chk_email_format CHECK(Email LIKE '%_@__%.__%')
);
CREATE INDEX User_Index ON ADD_USER(UserName);

--table to store every login history 
CREATE TABLE LoginHistory
(
	loginId SERIAL PRIMARY KEY,
	UserName varchar(50),
	Status VARCHAR(50),
	Email VARCHAR(50),
	Passwords VARCHAR(50),
	Photo BYTEA,
	LoginDate DATE
);
CREATE INDEX login_Index ON LoginHistory(UserName);

--table to store stock items
CREATE TABLE STOCK
(
	BatchId INT PRIMARY KEY NOT  NULL,
	ItemName VARCHAR(50) NOT NULL,
	Category VARCHAR(50) NOT NULL,
	QuantityStatus VARCHAR(30) NOT NULL,
	Quantity INT NOT NULL,
	Recommend VARCHAR(10) NOT NULL,
	CostPrice DECIMAL NOT NULL,
	SellingPrice DECIMAL NOT NULL,
	MfgDate DATE NOT NULL,
	ExpireDate DATE NOT NULL,
	Shell VARCHAR(20);
	Description VARCHAR(50),
	Photo BYTEA,
	Supplier VARCHAR(50),
	SupplierNo VARCHAR(15),
	
	constraint chk_costPrice CHECK(CostPrice > 0.00),
	constraint chk_sellPrice CHECK(SellingPrice > 0.00),
);
CREATE INDEX stock_Index ON STOCK(ItemName);

--table to store payments
CREATE TABLE Payments
(
	PaymentId INT SERIAL PRIMARY KEY,
	Tax DECIMAL,
	Discount DECIMAL,
	Gross DECIMAL NOT NULL,
	Total DECIMAL NOT NULL,
	Methods VARCHAR(45),
	Cash DECIMAL NOT NULL,
	AccountNo VARCHAR(50),
	PaidNo VARCHAR(15),
	Change DECIMAL,
	CustName VARCHAR(45) NOT NULL,
	Phone VARCHAR(15),
	DailySale DECIMAL,
);

--table to store sales 'pull thier values from stock table'
CREATE TABLE SALES
(
	SalesId INT PRIMARY KEY NOT NULL,
	BatchCode VARCHAR(50) NOT NULL,
	ItemName VARCHAR(50) NOT NULL,
	Category VARCHAR(50) NOT NULL,
	QuantityStatus VARCHAR(30) NOT NULL,
	Quantity INT NOT NULL,
	Recommend VARCHAR(10) NOT NULL,
	SellingPrice DECIMAL NOT NULL,
	ExpireDate DATE NOT NULL,
	Shell VARCHAR(20);
	Description VARCHAR(50) NOT NULL
	FOREIGN KEY(PaymentId) REFERENCES Payments(PaymentId),
	SaleDate DATE,
	CONSTRAINT chk_quantity CHECK(Quantity > 0),
);
CREATE INDEX sale_Index ON SALES(ItemName);
