#controls all aspects of the drug
class DrugsController < ApplicationController
  def new

  end

  def index
    @drugs = Drug.all
    if(params[:exist])
      @exist = true
    end

    
    @selected_drugs = params[:compares]

    if @selected_drugs != nil && @selected_drugs.length != 2
      flash.now[:notice] = "Please compare two drugs at a time."
    elsif @selected_drugs
      redirect_to drugs_compare_path(ids: @selected_drugs)
    end
  end

  def compare
    @drugs = Drug.where id: params[:ids].keys
  end

  def create
    @drug = Drug.new(drug_params)
    if !(Drug.find_by title: @drug.title)
      @drug.save
      redirect_to @drug
    else
      redirect_to drugs_path(:exist => true)
    end
  end

  def show
      @drug = Drug.find(params[:id])
  end

  def update

    @drug = Drug.find_by title: (params[:drug][:title])
    begin
      @drug.destroy
      rescue
    end
    @drug = Drug.new(drug_params)
    @drug.save
    redirect_to @drug
  end
  
  def edit
    @drug = Drug.find params[:id]
  end
  
  def destroy
    @drug = Drug.find(params[:id])
    @drug.destroy
    flash[:notice] = "Drug '#{@drug.title}' deleted."
    redirect_to drugs_path
  end
  
private
      def drug_params
        params.require(:drug).permit(:title, :category, :active_ingredient, :uses, :warnings, :directions, :other_information, :inactive_ingredients, :over_the_counter)
      end
end
