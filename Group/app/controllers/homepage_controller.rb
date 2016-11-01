#homepage controller
class HomepageController < ApplicationController
  def index
      @drugs = Drug.all
      @searched_drug = Drug.find_by title: params[:search_field]
  end

  def new
    # default: render 'new' template
  end
end
