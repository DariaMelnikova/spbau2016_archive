import System.Random
import System.IO.Unsafe
import Control.Monad
import System.IO
import Data.Function
import Data.List

genRandString :: IO String
genRandString = do
		g <- newStdGen
		let num = fst $ randomR (6,9) g
		return $ take num (randomRs ('Q', 'Z') g)


genRandList :: [String]
genRandList = genList 700
             where genList 0 = []
                   genList n = (unsafePerformIO genRandString) : (genList $ n-1)

writeF :: Handle -> [String] -> IO ()
writeF handle [] = return ()
writeF handle (x:xs) = do
					hPutStrLn handle x
					writeF handle xs

writeInitialData :: IO ()
writeInitialData = do
		let txtList = genRandList
		handle <- openFile "data.txt" WriteMode
		writeF handle txtList
		hClose handle
		return ()


processWord :: String -> Bool
processWord [] = False
processWord [x] = False
processWord [x,y] = if x == y then True else False
processWord (x:y:xs) = if x == y then True else processWord (y:xs)


processData :: [String] -> [String]
processData xs = sort $ filter processWord xs

readData :: Handle -> IO [String]
readData handle = do
		eof <- hIsEOF handle
		ifEndOfFile eof
    where ifEndOfFile True = return []
          ifEndOfFile False = do
                 str <- hGetLine handle
                 list <- readData handle
                 return $ str:list

main = do
	writeInitialData
	handle <- openFile "data.txt" ReadMode
	list <- readData handle
	hClose handle

	handleW <- openFile "result.txt" WriteMode
	writeF handleW (processData list)
	hClose handleW
	return ()
	
