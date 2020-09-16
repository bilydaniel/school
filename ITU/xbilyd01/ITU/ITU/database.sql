-- phpMyAdmin SQL Dump
-- version 4.0.10.20
-- https://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 08, 2019 at 10:48 PM
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
CREATE DATABASE IF NOT EXISTS `xmarek69` DEFAULT CHARACTER SET latin2 COLLATE latin2_czech_cs;
USE `xmarek69`;

-- --------------------------------------------------------

--
-- Table structure for table `itu_nastaveni`
--

DROP TABLE IF EXISTS `itu_nastaveni`;
CREATE TABLE IF NOT EXISTS `itu_nastaveni` (
  `id_nastaveni` int(11) NOT NULL AUTO_INCREMENT,
  `mnozstvi` int(11) NOT NULL,
  `dodavatel` int(11) NOT NULL,
  `cena_nakupni` int(11) NOT NULL,
  `cena_prodejni` int(11) NOT NULL,
  `mj` int(11) NOT NULL,
  `id_uzivatele` int(11) NOT NULL,
  `nazev` int(11) NOT NULL,
  PRIMARY KEY (`id_nastaveni`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=2 ;

--
-- Dumping data for table `itu_nastaveni`
--

INSERT INTO `itu_nastaveni` (`id_nastaveni`, `mnozstvi`, `dodavatel`, `cena_nakupni`, `cena_prodejni`, `mj`, `id_uzivatele`, `nazev`) VALUES
(1, 1, 1, 1, 0, 1, 1, 1);

-- --------------------------------------------------------

--
-- Table structure for table `itu_pohyb_zbozi`
--

DROP TABLE IF EXISTS `itu_pohyb_zbozi`;
CREATE TABLE IF NOT EXISTS `itu_pohyb_zbozi` (
  `id_pohybu` int(11) NOT NULL AUTO_INCREMENT,
  `id_uzivatele` int(11) NOT NULL,
  `datum` date NOT NULL,
  `poznamka` varchar(500) COLLATE latin2_czech_cs NOT NULL,
  `smer` int(11) NOT NULL,
  PRIMARY KEY (`id_pohybu`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=2 ;

--
-- Dumping data for table `itu_pohyb_zbozi`
--

INSERT INTO `itu_pohyb_zbozi` (`id_pohybu`, `id_uzivatele`, `datum`, `poznamka`, `smer`) VALUES
(1, 1, '2019-12-08', '', 0);

-- --------------------------------------------------------

--
-- Table structure for table `itu_uzivatel`
--

DROP TABLE IF EXISTS `itu_uzivatel`;
CREATE TABLE IF NOT EXISTS `itu_uzivatel` (
  `id_uzivatele` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(100) COLLATE latin2_czech_cs NOT NULL,
  `user_password` varchar(100) COLLATE latin2_czech_cs NOT NULL,
  PRIMARY KEY (`id_uzivatele`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=2 ;

--
-- Dumping data for table `itu_uzivatel`
--

INSERT INTO `itu_uzivatel` (`id_uzivatele`, `username`, `user_password`) VALUES
(1, 'a', 'a');

-- --------------------------------------------------------

--
-- Table structure for table `itu_zbozi`
--

DROP TABLE IF EXISTS `itu_zbozi`;
CREATE TABLE IF NOT EXISTS `itu_zbozi` (
  `id_zbozi` int(11) NOT NULL AUTO_INCREMENT,
  `id_pohybu` int(11) NOT NULL,
  `nazev` varchar(200) COLLATE latin2_czech_cs DEFAULT NULL,
  `dodavatel` varchar(200) COLLATE latin2_czech_cs NOT NULL,
  `cena_nakupni` int(11) NOT NULL,
  `cena_prodejni` int(11) NOT NULL,
  `mj` varchar(20) COLLATE latin2_czech_cs NOT NULL,
  `mnozstvi` int(11) NOT NULL,
  PRIMARY KEY (`id_zbozi`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs AUTO_INCREMENT=2 ;

--
-- Dumping data for table `itu_zbozi`
--

INSERT INTO `itu_zbozi` (`id_zbozi`, `id_pohybu`, `nazev`, `dodavatel`, `cena_nakupni`, `cena_prodejni`, `mj`, `mnozstvi`) VALUES
(1, 1, 'Kalhoty', 'Kalhotnici.sro', 200, 0, 'kus', 20);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
