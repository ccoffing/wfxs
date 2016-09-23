#ifndef XS_SELECTION_PATH_H
#define XS_SELECTION_PATH_H

#include <vector>

class BitMask;

/**
 *  @brief The "path" that outlines a selection.
 *
 *  Interface class for selection paths.
 *  A selection path has several important properties:
 *  - When completed, it is a closed path enclosing non-zero area.
 *  - It never crosses, or even touches, itself.
 */
class XSSelectionPath {
public:
    enum MoveDirection {
        // Code optimization expects opposite directions to have opposite signs.
        LEFT = -1,
        RIGHT = 1,
        UP = -2,
        DOWN = 2
    };

    virtual ~XSSelectionPath()
    {
    }

    /**
     *  Returns a new mask that describes the selection path.  All elements within the path are
     *  selected by the mask; all elements outside the path are unselected.  The mask is sized only
     *  large enough to enclose the selection.  Ownership of mask transfers to caller.
     *  @return Newly allocated mask describing selection.
     */
    virtual BitMask* GetMask() = 0;

    /**
     *  Attempts to move by a single square in the specified direction.
     *  @return true if the move was sucessful, otherwise false.  A move will fail if it would cause
     *      the existing path to be crossed, if it exceeds the boundaries specified in the
     *      constructor, or if the path is already complete.
     */
    virtual bool Move(MoveDirection m) = 0;

    /**
     *  Determines if the path is complete.  The precise definition of "complete" depends on the
     *  subclass.  Once a path is complete, it cannot be modified further.
     *  @return true if the path is complete, otherwise false.
     */
    virtual bool IsCompleted() = 0;

    /**
     *  Attempts to complete the path.
     *  @return true if the path was able to be completed; false if this failed for some reason.
     */
    virtual bool Complete() = 0;

    /**
     *  Returns the dimensions (width and height) of the completed selection path.
     *  @param[in,out] width The width.
     *  @param[in,out] height The height.
     */
    virtual void GetDimensions(unsigned int* width, unsigned int* height) = 0;

    /**
     *
     */
    virtual void GetEnd(unsigned int* x, unsigned int* y) = 0;

protected:
    static void Move(MoveDirection move, int* x, int* y);
};


/**
 *  @brief A free-form selection path.
 *
 *  A selection path has several important properties:
 *  - When completed, it is a closed path enclosing non-zero area.
 *  - It never crosses, or even touches, itself.
 */
class XSFreeFormSelectionPath : public XSSelectionPath {
public:
    /**
     *  Creates an empty selection path.
     */
    XSFreeFormSelectionPath();

    BitMask* GetMask();

    /**
     *  Attempts to move by a single square in the specified direction.
     *  @return true if the move was sucessful, otherwise false.  A move will fail if it would cause
     *      the existing path to be crossed, if it exceeds the boundaries specified in the
     *      constructor, or if the path is already complete.
     */
    bool Move(MoveDirection m);

    /**
     *  Determines if the path is complete.  A complete path is one that has returned to the
     *  starting point.  Once a path is complete, it cannot be modified further.
     *  @return true if the path is complete, otherwise false.
     */
    bool IsCompleted();

    /**
     *  Attempts to complete the path.
     *  @return true if the path was able to be completed; false if this failed for some reason.
     */
    bool Complete();

    void GetDimensions(unsigned int* width, unsigned int* height);

    void GetEnd(unsigned int* x, unsigned int* y);

protected:
    void UpdateCache();

    /**
     *  Walks the current path and discovers some characteristics of it.
     *  param[out] width Returns the width (difference between the minimal X coordinate and the
     *      maximal X coordinate) of the path.
     *  param[out] height Returns the height of the path.
     *  param[out] dx2  X offset of current ending point.
     *  param[out] dy2  Y offset of current ending point.
     *  param[out] dx1  Optional; X offset of starting point.
     *  param[out] dy1  Optional; Y offset of starting point.
     */
    void WalkPath(
            unsigned int* width, unsigned int* height, int* dx2, int* dy2, int* dx1, int* dy1);

    std::vector<MoveDirection> m_moves;
    unsigned int m_width, m_height;
    int m_x1, m_y1;  // Starting offsets (may change as path progresses and moves origin)
    int m_x, m_y;
    bool m_complete;
    bool m_validCache;
};


/**
 *  @brief A rectangular selection path.
 *
 *  A selection path has several important properties:
 *  - When completed, it is a closed path enclosing non-zero area.
 *  - It never crosses, or even touches, itself.
 */
class XSRectangularSelectionPath : public XSSelectionPath {
public:
    /**
     *  Creates an empty selection path.
     */
    XSRectangularSelectionPath();

    BitMask* GetMask();

    bool Move(MoveDirection m);

    /**
     *  Determines if the path is complete.  Once a path is complete, it cannot be modified further.
     *  @return true if the path is complete, otherwise false.
     */
    bool IsCompleted();

    /**
     *  Marks the path as complete.
     *  @return true (always succeeds for rectangular selection path)
     */
    bool Complete();

    void GetDimensions(unsigned int* width, unsigned int* height);

    void GetEnd(unsigned int* x, unsigned int* y);

private:
    int m_x, m_y;
    bool m_complete;
};

#endif
