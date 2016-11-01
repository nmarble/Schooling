# each drug has many titles
class Drug < ActiveRecord::Base
   has_many :titles
end
