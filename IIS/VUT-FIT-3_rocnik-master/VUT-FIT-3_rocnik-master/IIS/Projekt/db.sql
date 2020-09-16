-- phpMyAdmin SQL Dump
-- version 4.0.10.20
-- https://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Nov 30, 2019 at 06:01 PM
-- Server version: 5.6.40
-- PHP Version: 5.3.29

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `xmarek69`
--
-- CREATE DATABASE IF NOT EXISTS `xmarek69` DEFAULT CHARACTER SET latin2 COLLATE latin2_czech_cs;
-- USE `xmarek69`;

-- --------------------------------------------------------

--
-- Table structure for table `druh_jidla`
--

DROP TABLE IF EXISTS `druh_jidla`;
CREATE TABLE IF NOT EXISTS `druh_jidla` (
  `druh_polozky` varchar(100) COLLATE latin2_czech_cs NOT NULL,
  `id_polozky` int(11) NOT NULL,
  PRIMARY KEY (`druh_polozky`,`id_polozky`)
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

--
-- Dumping data for table `druh_jidla`
--

INSERT INTO `druh_jidla` (`druh_polozky`, `id_polozky`) VALUES
('gluten_free', 3),
('vegan', 2),
('vegetarian', 2);

-- --------------------------------------------------------

--
-- Table structure for table `objednana_polozka`
--

DROP TABLE IF EXISTS `objednana_polozka`;
CREATE TABLE IF NOT EXISTS `objednana_polozka` (
  `id_objednane_polozky` int(11) NOT NULL AUTO_INCREMENT,
  `id_objednavky` int(11) NOT NULL,
  `id_polozky` int(11) NOT NULL,
  `mnozstvi` int(11) NOT NULL,
  PRIMARY KEY (`id_objednane_polozky`)
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `objednavka`
--

DROP TABLE IF EXISTS `objednavka`;
CREATE TABLE IF NOT EXISTS `objednavka` (
  `id_objednavky` int(11) NOT NULL AUTO_INCREMENT,
  `id_osoby` int(11) NOT NULL,
  `id_ridice` int(11) NOT NULL,
  `id_provozovny` int(11) NOT NULL,
  `cas` time NOT NULL,
  `stav` int(1) NOT NULL DEFAULT '1',
  `cena` int(11) NOT NULL,
  PRIMARY KEY (`id_objednavky`)
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `osoba`
--

DROP TABLE IF EXISTS `osoba`;
CREATE TABLE IF NOT EXISTS `osoba` (
  `id_osoby` int(11) NOT NULL AUTO_INCREMENT,
  `jmeno` varchar(100) CHARACTER SET utf8 NOT NULL,
  `prijmeni` varchar(100) CHARACTER SET latin2 COLLATE latin2_czech_cs NOT NULL,
  `telefon` varchar(15) CHARACTER SET latin2 COLLATE latin2_czech_cs NOT NULL,
  `email` varchar(200) CHARACTER SET latin2 COLLATE latin2_czech_cs NOT NULL,
  `adresa` varchar(500) CHARACTER SET latin2 COLLATE latin2_czech_cs NOT NULL,
  PRIMARY KEY (`id_osoby`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci AUTO_INCREMENT=5 ;

--
-- Dumping data for table `osoba`
--

INSERT INTO `osoba` (`id_osoby`, `jmeno`, `prijmeni`, `telefon`, `email`, `adresa`) VALUES
(1, 'Karel', '', '123', '', '123'),
(2, 'Pepa', '', '456', '', '456'),
(3, 'Lukáš', '', '789', '', '789'),
(4, 'Josef', '', '963', '', '963');

-- --------------------------------------------------------

--
-- Table structure for table `polozka`
--

DROP TABLE IF EXISTS `polozka`;
CREATE TABLE IF NOT EXISTS `polozka` (
  `id_polozky` int(11) NOT NULL AUTO_INCREMENT,
  `nazev` varchar(200) COLLATE latin2_czech_cs NOT NULL,
  `cena` int(11) NOT NULL,
  `popis` varchar(500) COLLATE latin2_czech_cs NOT NULL,
  `trvala_polozka` int(11) NOT NULL DEFAULT '0',
  `id_provozovny` int(11) NOT NULL,
  `zbyva` int(11) NOT NULL DEFAULT '0',
  `img` varchar(200) COLLATE latin2_czech_cs NOT NULL DEFAULT 'http://www.stud.fit.vutbr.cz/~xmarek69/IIS/assets/img/food.png',
  PRIMARY KEY (`id_polozky`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=4 ;

--
-- Dumping data for table `polozka`
--

INSERT INTO `polozka` (`id_polozky`, `nazev`, `cena`, `popis`, `trvala_polozka`, `id_provozovny`, `zbyva`, `img`) VALUES
(1, 'Řízek s brambory', 120, 'Kuřecí řízek se šťouchanými brambory a cibulkou', 1, 1, 1, 'http://www.stud.fit.vutbr.cz/~xmarek69/IIS/assets/img/food.png'),
(2, 'Zeleninový salát', 85, 'Okurka, rajče, paprika a další zelenina', 0, 1, 5, 'http://www.stud.fit.vutbr.cz/~xmarek69/IIS/assets/img/food.png'),
(3, 'Salámová', 220, 'Pizza se všemi druhy salámu', 1, 2, 1, 'http://www.stud.fit.vutbr.cz/~xmarek69/IIS/assets/img/food.png');

-- --------------------------------------------------------

--
-- Table structure for table `provozovna`
--

DROP TABLE IF EXISTS `provozovna`;
CREATE TABLE IF NOT EXISTS `provozovna` (
  `id_provozovny` int(11) NOT NULL AUTO_INCREMENT,
  `adresa` varchar(100) COLLATE latin2_czech_cs NOT NULL,
  `oteviraci_doba` time NOT NULL,
  `zaviraci_doba` time NOT NULL,
  `nazev` varchar(300) COLLATE latin2_czech_cs NOT NULL,
  `popis` varchar(500) COLLATE latin2_czech_cs NOT NULL,
  `telefon` varchar(12) COLLATE latin2_czech_cs NOT NULL,
  `img` varchar(200) COLLATE latin2_czech_cs NOT NULL DEFAULT 'http://www.stud.fit.vutbr.cz/~xmarek69/IIS/assets/img/restaurant.png',
  PRIMARY KEY (`id_provozovny`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=3 ;

--
-- Dumping data for table `provozovna`
--

INSERT INTO `provozovna` (`id_provozovny`, `adresa`, `oteviraci_doba`, `zaviraci_doba`, `nazev`, `popis`, `telefon`, `img`) VALUES
(1, 'Lesní 12', '08:00:00', '22:00:00', 'Restaurace U Jezera', 'Nejklidnější restaurace v okolí', '159951', 'http://www.stud.fit.vutbr.cz/~xmarek69/IIS/assets/img/restaurant.png'),
(2, 'Okružní 5', '10:00:00', '23:00:00', 'Pizzerie na rohu', 'Máme nejtenší pizzu na světě', '147896', 'http://www.stud.fit.vutbr.cz/~xmarek69/IIS/assets/img/restaurant.png');

-- --------------------------------------------------------

--
-- Table structure for table `stravnik`
--

DROP TABLE IF EXISTS `stravnik`;
CREATE TABLE IF NOT EXISTS `stravnik` (
  `id_stravnika` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(100) COLLATE latin2_czech_cs NOT NULL,
  `heslo` varchar(200) COLLATE latin2_czech_cs NOT NULL,
  `role` varchar(100) COLLATE latin2_czech_cs NOT NULL DEFAULT 'user',
  `id_osoby` int(11) NOT NULL,
  PRIMARY KEY (`id_stravnika`),
  KEY `id_osoby` (`id_osoby`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=5 ;

--
-- Dumping data for table `stravnik`
--

INSERT INTO `stravnik` (`id_stravnika`, `login`, `heslo`, `role`, `id_osoby`) VALUES
(1, 'admin', 'admin', 'admin', 1),
(2, 'operator', 'operator', 'operator', 2),
(3, 'ridic', 'ridic', 'driver', 3),
(4, 'uzivatel', 'uzivatel', 'user', 4);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `stravnik`
--
ALTER TABLE `stravnik`
  ADD CONSTRAINT `stravnik_ibfk_1` FOREIGN KEY (`id_osoby`) REFERENCES `osoba` (`id_osoby`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
