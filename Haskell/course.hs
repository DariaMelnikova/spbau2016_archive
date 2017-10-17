import Control.Applicative
import

type Symb = String 
infixl 2 :@

data Expr = Var Symb
          | Expr :@ Expr
          | Lam Symb Expr
          deriving (Eq, Read, Show)
freeVars :: Expr -> [Symb]
freeVars (Var s) = [s]
freeVars (f :@ a) = freeVars f `union` freeVars a
freeVars (Lam a expr) = filter (/= a) (freeVars expr)

subst :: Symb -> Expr -> Expr -> Expr
subst v x b = sub b
  where sub e@(Var i) = if i == v then x else e
        sub (f :@ a) =  (sub f) :@ (sub a)
        sub (Lam i e) =
            if v == i then
                Lam i e
            else if i `elem` fvx then
                let i' = cloneSym e i
                    e' = substVar i i' e
                in  Lam i' (sub e')
            else
                Lam i (sub e)
        fvx = freeVars x
        cloneSym e i = loop i
           where loop i' = if i' `elem` vars then loop (i ++ "'") else i'
                 vars = fvx ++ freeVars e

substVar :: Symb -> Symb -> Expr -> Expr
substVar s s' e = subst s (Var s') e

infix  1 `alphaEq`
alphaEq :: Expr -> Expr -> Bool
alphaEq (Var a)   (Var b)    = a == b
alphaEq (a :@ b) (c :@ d)    = (alphaEq a c) && (alphaEq b d)
alphaEq (Lam a expr1) (Lam b expr2) = alphaEq expr2 (subst a (Var b) expr1)
alphaEq _ _ = False

reduce'n'check a b = case reduceOnce b of
                      Just rb -> Just $ a :@ rb
                      Nothing -> Nothing

reduceOnce :: Expr -> Maybe Expr
reduceOnce ((Lam a expr1) :@ x) = Just $ subst a x expr1
reduceOnce (Lam a expr) = case reduceOnce expr of
                          Just n -> Just $ Lam a n
                          Nothing -> Nothing
reduceOnce (a :@ b) = case reduceOnce a of
                      Just ra -> Just $ ra :@ b
                      Nothing -> reduce'n'check a b

reduceOnce _ = Nothing


reduce:: Expr -> Expr
reduce (Var a) = Var a
reduce (Lam a b) = Lam a (reduce b)
reduce ((Lam a b) :@ c)= reduce $ subst a c b 
reduce (a :@ b) = (reduce a) :@ (reduce b)

help (Lam a b) [] = Lam a (nf b)
help (Lam a b) (x:xs) = help (subst a x b) xs
help (a :@ b) x = help a (b:x)
help a xs = foldl (:@) a (map nf xs)



nf :: Expr -> Expr
nf expr | expr == reduce expr = expr
        | otherwise = nf $ reduce expr



[a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z] = 
  map (Var . (:[])) "abcdefghijklmnopqrstuvwxyz"
-- аппликация двух аргументов
app2 f x y = f :@ x :@ y
-- аппликация трёх аргументов
app3 f x y z = f :@ x :@ y :@ z

-- комбинаторы
cI     = Lam "x" x
cK     = Lam "x" $ Lam "y" x
cK_ast = Lam "x" $ Lam "y" y
cB     = Lam "f" $ Lam "g" $ Lam "x" $ f :@ (g :@ x)
cS     = Lam "f" $ Lam "g" $ Lam "x" $ f :@ x :@ (g :@ x)

-- Булевы значения
fls = Lam "t" $ Lam "f" f
tru = Lam "t" $ Lam "f" t

iif = Lam "b" $ Lam "x" $ Lam "y" $ b :@ x :@ y

not' = Lam "b" $ Lam "t" $ Lam "f" $ app2 b f t
and' = Lam "x" $ Lam "y" $ app2 x y fls
or'  = Lam "x" $ Lam "y" $ app2 x tru y

-- пары
pair = Lam "x" $ Lam "y" $ Lam "f" $ app2 f x y

fst' = Lam "p" $ p :@ tru
snd' = Lam "p" $ p :@ fls

-- числа Чёрча
zero  = Lam "s" $ Lam "z" z
one   = Lam "s" $ Lam "z" $ s :@ z
two   = Lam "s" $ Lam "z" $ s :@ (s :@ z)
three = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ z))
four  = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ z)))
five  = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ z))))
six   = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z)))))
seven = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z))))))
eight = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z)))))))
nine  = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z))))))))
ten   = Lam "s" $ Lam "z" $ s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ (s :@ z)))))))))
  
iszro = Lam "n" $ n :@ (Lam "x" fls) :@ tru

suc = Lam "n" $ Lam "s" $ Lam "z" $  s :@ (n :@ s :@ z)

plus  = Lam "m" $ Lam "n" $ Lam "s" $ Lam "z" $ app2 m s (app2 n s z)

mult = Lam "m" $ Lam "n" $ Lam "s" $ m :@ (n :@ s)

pow = Lam "m" $ Lam "n" $ n :@ m

omega = Lam "x" $ x :@ x

zp = pair :@ zero :@ zero
sp = Lam "p" $  pair :@ (snd' :@ p) :@ (suc :@ (snd' :@ p))
pred' = Lam "m" $ fst' :@ (m :@ sp :@ zp)

-- факториал
zf = pair :@ one :@ zero
sf = Lam "p" $ pair :@ (mult :@ (fst' :@  p) :@ (suc :@ (snd' :@ p))) :@ (suc :@ (snd' :@ p))
fac = Lam "m" $ fst' :@ (m :@ sf :@ zf)

-- общая схема примитивной рекурсии
xz = Lam "x" $ pair :@ x :@ zero
fs = Lam "f" $ Lam "p" $ pair :@ (f :@ (fst' :@ p) :@ (snd' :@ p)) :@ (suc :@ (snd' :@ p))
rec = Lam "m" $ Lam "f" $ Lam "x" $ fst' :@ (m :@ (fs :@ f) :@ (xz :@ x))

pred'' = Lam "n" $ rec :@ n :@ cK_ast :@ zero

minus = Lam "k" $ Lam "l" $ l :@ pred' :@ k

lt = Lam "n" $ Lam "m" $ not' :@ (iszro :@ (minus :@ m :@ n))
ge = Lam "n" $ Lam "m" $  iszro :@ (app2 minus m n)
gt = Lam "n" $ Lam "m" $ not' :@ (iszro :@ (app2 minus n m))
le = Lam "n" $ Lam "m" $  iszro :@ (app2 minus n m)
eq = Lam "n" $ Lam "m" $ and' :@ (le :@ m :@ n) :@ (ge :@ m :@ n)

fac'step = Lam "u" $ Lam "v" $ app2 mult u (suc :@ v)
fac' = Lam "n" $ app3 rec n fac'step one

-- Y-комбинатор
cY = Lam "f" $ (Lam "z" $ f :@ (z :@ z)) :@ (Lam "z" $ f :@ (z :@ z)) 
cTheta = aa :@ aa 
  where aa = Lam "a" $ Lam "b" $ b :@ (a :@ a :@ b)

fac''step = Lam "f" $ Lam "n" $ iif :@ (iszro :@ n) :@ one :@ (mult :@ n :@ (f :@ (pred' :@ n))) 
fac''  =  cY :@ fac''step
fac''' = cTheta :@ fac''step

-- списки
nil  = Lam "c" $ Lam "n" n
cons = Lam "e" $ Lam "l" $ Lam "c" $ Lam "n" $ app2 c e (app2 l c n)

l532 = app2 cons five (app2 cons three (app2 cons two nil))
l2 = Lam "c" $ Lam "n" $ c :@ two :@ n

empty = Lam "l" $ app2 l (Lam "h" $ Lam "t" fls) tru

length' = Lam "l" $ app2 l (Lam "x" $ Lam "y" $ suc :@ y) zero
length'' = Lam "l" $ app2 l (Lam "y" $ suc) zero

head' = Lam "l" $ app2 l cK cI

zpl = app2 pair nil nil
spl = Lam "e" $ Lam "p" $ app2 pair (snd' :@ p) (app2 cons e (snd' :@ p))
tail' = Lam "l" $ fst' :@ (app2 l spl zpl)

sum' = Lam "l" $ app2 l plus zero
sum'' = Lam "l" $ app2 l (Lam "h" $ Lam "t" $ app2 plus h t) zero


